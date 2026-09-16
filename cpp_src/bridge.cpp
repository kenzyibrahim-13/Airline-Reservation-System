#include "bridge.h"
#include "AuthSystem.h"
#include "SystemManager.h"
#include "Admin.h"
#include "Passenger.h"
#include "Flight.h"
#include "Ticket.h"
#include "FileManager.h"
#include "SuperAdmin.h"
#include <cstring>          // For strncpy (used in the safe_copy helper)

static AuthSystem auth;                    // Single AuthSystem instance used by all auth functions
static Admin*      currentAdmin     = nullptr;   // Pointer to the currently logged-in admin (null if none)
static Passenger*  currentPassenger = nullptr;   // Pointer to the currently logged-in passenger (null if none)
static SuperAdmin* Sup              = nullptr;   // Pointer to the currently logged-in super admin (null if none)


// Helper to safely copy a std::string into a fixed-size char buffer
// maxLen defaults to 256; the last byte is always set to '\0' to guarantee null-termination
static void safe_copy(char* dest, const string& src, int maxLen = 256) {
    strncpy(dest, src.c_str(), maxLen - 1);   // Copy at most maxLen-1 characters
    dest[maxLen - 1] = '\0';                   // Force null-terminator at the very end
}

extern "C" {   // All functions below are exported with C linkage (no C++ name mangling)

// --- AUTH -----------------------------------------------------------------

// Registers a new passenger with the provided credentials
// Returns 1 if sign-up succeeded, 0 if it failed (e.g. duplicate national ID)
EXPORT int passenger_signup(const char* name, const char* id, const char* phone,
                             const char* email, const char* pass) {
    return auth.passengerSignup(name, id, phone, email, pass) ? 1 : 0;   // Delegate to AuthSystem; convert bool to int
}

// Authenticates a passenger and stores a pointer to them in currentPassenger
// Returns 1 on success, 0 if ID not found or password is wrong.
EXPORT int passenger_login(const char* id, const char* pass) {
    currentPassenger = auth.passengerLogin(id, pass);   // Attempt login; result is nullptr on failure
    return currentPassenger != nullptr ? 1 : 0;          // Return 1 if login succeeded
}

// Authenticates an admin (validates against the master password) and stores the pointer
// Returns 1 on success, 0 if the password is wrong or the ID does not exist
EXPORT int admin_login(const char* id, const char* pass) {
    currentAdmin = auth.adminLogin(id, pass);   // Attempt login; result is nullptr on failure
    return currentAdmin != nullptr ? 1 : 0;      // Return 1 if login succeeded
}

// Authenticates the super admin and stores the pointer in Sup
// Returns 1 on success, 0 on failure
EXPORT int super_admin_login(const char* id, const char* pass){
    Sup = SuperAdmin::login(id, pass);   // Use the SuperAdmin static login method
    return Sup != nullptr ? 1 : 0;       // Return 1 if login succeeded
}

// --- FLIGHTS ----------------------------------------------------------------

// Adds a new flight to the system if no flight with the same ID already exists
// Returns 1 on success, 0 if a duplicate ID is found
EXPORT int admin_add_flight(const char* id, const char* from, const char* to,
                             int cap, double price, const char* date, const char* time) {
    int flightID = atoi(id);                                                   // Convert the string ID to an integer
    if (SystemManager::getInstance()->searchFlightID(flightID)) return 0;     // Reject if flight ID already exists
    SystemManager::getInstance()->addFlight(
        new Flight(flightID, from, to, cap, price, date, time));               // Create and register the new flight
    return 1;   // Return success
}

// Cancels (removes) the flight with the given numeric ID
// Returns 1 if the flight was found and removed, 0 otherwise
EXPORT int admin_cancel_flight(int flightID) {
    return SystemManager::getInstance()->cancelFlight(flightID) ? 1 : 0;   // Delegate; convert bool to int
}

// Returns the total number of flights currently registered in the system
EXPORT int get_flight_count() {
    return (int)SystemManager::getInstance()->getAllFlights().size();   // Cast size_t to int for FFI compatibility
}

// Fills caller-supplied output buffers with the data for the flight at the given index
// Does nothing if the index is out of range.
EXPORT void get_flight_by_index(int index, char* out_id, char* out_from, char* out_to,
                                  char* out_date, char* out_time,
                                  double* out_price, int* out_is_full) {
    auto flights = SystemManager::getInstance()->getAllFlights();              // Get the full flight list
    if (index < 0 || index >= (int)flights.size()) return;                    // Bounds check; do nothing if invalid
    Flight* f = flights[index];                                               // Get the flight at the requested index
    safe_copy(out_id,   to_string(f->getFlightID()));                         // Copy flight ID string into buffer
    safe_copy(out_from, f->getOrigin());                                      // Copy origin city into buffer
    safe_copy(out_to,   f->getDestination());                                 // Copy destination city into buffer
    safe_copy(out_date, f->getDate());                                        // Copy departure date into buffer
    safe_copy(out_time, f->getTime());                                        // Copy departure time into buffer
    *out_price   = f->getPrice();                                             // Write ticket price via pointer
    *out_is_full = f->isFull() ? 1 : 0;                                      // Write full-status flag (1=full, 0=available)
}

// --- TICKETS -----------------------------------------------------------------

// Books a ticket for the given passenger on the given flight at the specified seat
// isBusinessClass: 1 for Business, 0 for Economy
// Returns the new ticket ID on success, or -1 on failure
EXPORT int book_ticket(const char* passengerID, int flightID,
                        const char* seat, int isBusinessClass) {
    TicketType type = isBusinessClass ? BUSINESS : ECONOMY;                               // Map int flag to enum
    Ticket* t = SystemManager::getInstance()->bookTicket(passengerID, flightID, seat, type);  // Attempt booking
    return t != nullptr ? t->getTicketID() : -1;                                          // Return ticket ID or -1
}

// Cancels the ticket with the given ID and frees the associated seat
// Returns 1 on success, 0 if the ticket was not found
EXPORT int cancel_ticket(int ticketID) {
    return SystemManager::getInstance()->removeTicket(ticketID) ? 1 : 0;   // Delegate; convert bool to int
}

// Returns the number of tickets currently held by the specified passenger
// Returns 0 if the passenger does not exist.
EXPORT int get_ticket_count_for_passenger(const char* passengerID) {
    Passenger* p = SystemManager::getInstance()->searchPassengerID(passengerID);   // Look up passenger
    if (!p) return 0;                                                               // Passenger not found
    return (int)p->getTickets().size();                                             // Return ticket count
}

// Fills caller-supplied output variables with data for the ticket at the given index for a passenger
// Does nothing if the passenger is not found or the index is out of range
EXPORT void get_ticket_by_index(const char* passengerID, int index,
                                  int* out_ticketID, int* out_flightID,
                                  char* out_seat, int* out_isBusiness,
                                  double* out_totalPrice) {
    Passenger* p = SystemManager::getInstance()->searchPassengerID(passengerID);   // Look up passenger
    if (!p) return;                                                                 // Passenger not found; do nothing
    auto tickets = p->getTickets();                                                 // Get the passenger's ticket list
    if (index < 0 || index >= (int)tickets.size()) return;                         // Bounds check
    Ticket* t = tickets[index];                                                     // Get the ticket at the given index
    *out_ticketID    = t->getTicketID();                                            // Write ticket ID via pointer
    *out_flightID    = t->getFlightID();                                            // Write flight ID via pointer
    safe_copy(out_seat, t->getSeatNumber());                                        // Copy seat label into buffer
    *out_isBusiness  = (t->getType() == BUSINESS) ? 1 : 0;                         // Write class flag (1=business)
    *out_totalPrice  = t->calculateTotalPrice();                                    // Write computed total price
}

// --- LUGGAGE ---------------------------------------------------------------

// Adds a luggage item to the specified ticket
// isCabin: 1 for cabin luggage, 0 for checked luggage
// Returns 1 on success, 0 if the ticket was not found
EXPORT int add_luggage_to_ticket(int ticketID, double weight, int isCabin) {
    SystemManager* sys = SystemManager::getInstance();           // Get the system manager singleton
    Ticket* t = sys->searchTicketID(ticketID);                   // Look up the ticket
    if (!t) return 0;                                            // Ticket not found; return failure
    LuggageType type = isCabin ? CABIN : CHECKED;                // Map int flag to LuggageType enum
    t->addLuggage(Luggage(weight, type));                        // Create and attach the luggage to the ticket
    return 1;                                                    // Return success
}

// Returns the total number of luggage items attached to the specified ticket
// Returns 0 if the ticket is not found
EXPORT int get_luggage_count_for_ticket(int ticketID) {
    SystemManager* sys = SystemManager::getInstance();           // Get the system manager singleton
    Ticket* t = sys->searchTicketID(ticketID);                   // Look up the ticket
    if (!t) return 0;                                            // Ticket not found; return 0
    return (int)t->getLuggages().size();                         // Return luggage count
}

// Returns the total combined weight of all luggage on the specified ticket
// Returns 0.0 if the ticket is not found
EXPORT double get_ticket_total_weight(int ticketID) {
    SystemManager* sys = SystemManager::getInstance();           // Get the system manager singleton
    Ticket* t = sys->searchTicketID(ticketID);                   // Look up the ticket
    if (!t) return 0.0;                                          // Ticket not found; return 0.0
    return t->getTotalWeight();                                  // Delegate total-weight calculation to Ticket
}

// Returns the total extra baggage fee for the specified ticket
// Returns 0.0 if the ticket is not found
EXPORT double get_ticket_extra_fee(int ticketID) {
    SystemManager* sys = SystemManager::getInstance();           // Get the system manager singleton
    Ticket* t = sys->searchTicketID(ticketID);                   // Look up the ticket
    if (!t) return 0.0;                                          // Ticket not found; return 0.0
    return t->calculateExtraFee();                               // Delegate extra-fee calculation to Ticket
}

// Returns 1 if any luggage on the ticket exceeds the free allowance, 0 otherwise
// Returns 0 if the ticket is not found
EXPORT int ticket_has_extra_fees(int ticketID) {
    SystemManager* sys = SystemManager::getInstance();           // Get the system manager singleton
    Ticket* t = sys->searchTicketID(ticketID);                   // Look up the ticket
    if (!t) return 0;                                            // Ticket not found; return 0
    return t->hasExtraFees() ? 1 : 0;                           // Return 1 if extra fees apply
}

// --- USERS -----------------------------------------------------------------

// Returns the total number of registered passengers in the system
EXPORT int get_user_count() {
    return (int)SystemManager::getInstance()->getAllPassengers().size();   // Cast size_t to int
}

// Fills caller-supplied buffers with the name and national ID of the passenger at the given index.
// Does nothing if the index is out of range.
EXPORT void get_user_by_index(int index, char* out_name, char* out_id) {
    auto passengers = SystemManager::getInstance()->getAllPassengers();    // Get the full passenger list
    if (index < 0 || index >= (int)passengers.size()) return;             // Bounds check
    safe_copy(out_name, passengers[index]->getName());                    // Copy passenger name into buffer
    safe_copy(out_id,   passengers[index]->getNationalID());              // Copy national ID into buffer
}

// --- PASSENGER ----------------------------------------------------------------

// Updates the phone number of the passenger with the given national ID
// Returns 1 on success, 0 if the passenger was not found
EXPORT int edit_passenger_phone(const char* passengerID, const char* newPhone) {
    Passenger* p = SystemManager::getInstance()->searchPassengerID(passengerID);   // Look up passenger
    if (!p) return 0;                                                               // Not found; return failure
    p->setPhoneNumber(string(newPhone));                                            // Update the phone number
    return 1;                                                                       // Return success
}

// Updates the email address of the passenger with the given national ID
// Returns 1 on success, 0 if the passenger was not found
EXPORT int edit_passenger_email(const char* passengerID, const char* newEmail) {
    Passenger* p = SystemManager::getInstance()->searchPassengerID(passengerID);   // Look up passenger
    if (!p) return 0;                                                               // Not found; return failure
    p->setEmail(string(newEmail));                                                  // Update the email address
    return 1;                                                                       // Return success
}

// Fills caller-supplied buffers with the name, email, and phone of the passenger with the given ID
// Does nothing if the passenger is not found
EXPORT void get_passenger_info(const char* id, char* out_name, char* out_mail, char* out_phone){
    Passenger* p = SystemManager::getInstance() -> searchPassengerID(id);   // Look up passenger
    if (!p) return;                                                           // Not found; do nothing
    safe_copy(out_name,  p->getName());                                      // Copy name into buffer
    safe_copy(out_mail,  p->getEmail());                                     // Copy email into buffer
    safe_copy(out_phone, p->getPassengerPhone());                            // Copy phone number into buffer
}

// --- SEATS -----------------------------------------------------------------

// Returns the number of available seats of the given class on the specified flight
// isBusinessClass: 1 for Business, 0 for Economy
// Returns 0 if the flight is not found
EXPORT int get_available_seat_count(int flightID, int isBusinessClass) {
    Flight* f = SystemManager::getInstance()->searchFlightID(flightID);   // Look up flight
    if (!f) return 0;                                                       // Flight not found; return 0
    TicketType type = isBusinessClass ? BUSINESS : ECONOMY;                 // Map int flag to TicketType enum
    return (int)f->getAvailableSeats(type).size();                          // Return count of available seats
}

// Fills out_seat with the seat label (e.g. "3B") at the given index for the specified flight and class
// Does nothing if the flight is not found or the index is out of range
EXPORT void get_seat_by_index(int flightID, int isBusinessClass,
                               int index, char* out_seat) {
    Flight* f = SystemManager::getInstance()->searchFlightID(flightID);   // Look up flight
    if (!f) return;                                                         // Flight not found; do nothing
    TicketType type = isBusinessClass ? BUSINESS : ECONOMY;                 // Map int flag to TicketType enum
    auto seats = f->getAvailableSeats(type);                                // Get available seat list for class
    if (index < 0 || index >= (int)seats.size()) return;                    // Bounds check
    safe_copy(out_seat, seats[index]);                                      // Copy seat label into buffer
}

// --- FileManager -------------------------------------------------------------

// Persists all current in-memory data (passengers, flights, tickets, luggage, admins) to CSV files
EXPORT void save_all_data(){
    FileManager::saveAllSystemData(SystemManager::getInstance());   // Delegate to FileManager static method
}

// Restores all previously saved data from CSV files back into memory
EXPORT void load_all_data(){
    FileManager::loadAllSystemData(SystemManager::getInstance());   // Delegate to FileManager static method
}

// --- SuperAdmin ----------------------------------------------------------------

// Creates a new Admin account using the User factory method
// Returns 1 if creation succeeded, 0 on failure (e.g. duplicate ID)
EXPORT int add_admin(const char* name, const char* id, const char* phone, const char* email) {
    User* u = User::build(ADMIN, name, id, phone, email, "");   // Build an Admin through the User factory (no password needed)
    return u != nullptr ? 1 : 0;                                 // Return 1 if creation succeeded
}

// Removes the admin with the given national ID from the system
// Returns 1 on success, 0 if the admin was not found
EXPORT int remove_admin(const char* id) {
    return SystemManager::getInstance()->removeAdmin(string(id)) ? 1 : 0;   // Delegate; convert bool to int
}

}
