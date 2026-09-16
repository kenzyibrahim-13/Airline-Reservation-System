#ifndef BRIDGE_H
#define BRIDGE_H

// Platform-specific export macro
// On Windows, __declspec(dllexport) marks a symbol as exported from a DLL
// On other platforms (Linux/macOS), visibility("default") achieves the same effect
#ifdef _WIN32
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT __attribute__((visibility("default")))
#endif

// ffigen (Flutter's FFI binding generator) parses this header as plain C,
// so the extern "C" block is only added when compiled as C++ to prevent name mangling
#ifdef __cplusplus
extern "C" {
#endif

// --- Auth ---
// Signs up a new passenger; returns 1 on success, 0 on failure
EXPORT int  passenger_signup(const char* name, const char* id, const char* phone, const char* email, const char* pass);
// Logs in a passenger by ID and password; returns 1 on success, 0 on failure
EXPORT int  passenger_login(const char* id, const char* pass);
// Logs in an admin using their ID and the shared master password; returns 1 on success, 0 on failure
EXPORT int  admin_login(const char* id, const char* pass);
// Logs in the super admin using their ID and password; returns 1 on success, 0 on failure
EXPORT int  super_admin_login(const char* id, const char* pass);

// --- Flights ---
// Adds a new flight to the system; returns 1 on success, 0 if the flight ID already exists
EXPORT int  admin_add_flight(const char* id, const char* from, const char* to, int cap, double price, const char* date, const char* time);
// Cancels (removes) an existing flight by its numeric ID; returns 1 on success, 0 on failure
EXPORT int  admin_cancel_flight(int flightID);
// Returns the total number of flights currently in the system
EXPORT int  get_flight_count();
// Fills caller-supplied buffers with data for the flight at the given list index
EXPORT void get_flight_by_index(int index, char* out_id, char* out_from, char* out_to, char* out_date, char* out_time, double* out_price, int* out_is_full);

// --- Tickets ---
// Books a ticket for a passenger on a flight; returns the new ticket ID, or -1 on failure
EXPORT int  book_ticket(const char* passengerID, int flightID, const char* seat, int isBusinessClass);
// Cancels a ticket by its ID; returns 1 on success, 0 on failure
EXPORT int  cancel_ticket(int ticketID);
// Returns the number of tickets held by the specified passenger
EXPORT int  get_ticket_count_for_passenger(const char* passengerID);
// Fills caller-supplied variables with data for the ticket at the given index for a passenger
EXPORT void get_ticket_by_index(const char* passengerID, int index, int* out_ticketID, int* out_flightID, char* out_seat, int* out_isBusiness, double* out_totalPrice);

// --- Luggage ---
// Adds a piece of luggage (cabin or checked) to the specified ticket; returns 1 on success, 0 on failure
EXPORT int    add_luggage_to_ticket(int ticketID, double weight, int isCabin);
// Returns the total number of luggage items attached to the specified ticket
EXPORT int    get_luggage_count_for_ticket(int ticketID);
// Returns the total combined weight (kg) of all luggage on the specified ticket
EXPORT double get_ticket_total_weight(int ticketID);
// Returns the total extra baggage fee charged for the specified ticket
EXPORT double get_ticket_extra_fee(int ticketID);
// Returns 1 if the ticket has any luggage that incurs extra fees, 0 otherwise
EXPORT int    ticket_has_extra_fees(int ticketID);

// --- Users ---
// Returns the total number of registered passengers in the system
EXPORT int  get_user_count();
// Fills caller-supplied buffers with the name and national ID of the passenger at the given index
EXPORT void get_user_by_index(int index, char* out_name, char* out_id);

// --- Passenger ---
// Updates the phone number of the passenger with the given ID; returns 1 on success, 0 on failure
EXPORT int  edit_passenger_phone(const char* passengerID, const char* newPhone);
// Updates the email address of the passenger with the given ID; returns 1 on success, 0 on failure
EXPORT int  edit_passenger_email(const char* passengerID, const char* newEmail);
// Fills caller-supplied buffers with the name, email, and phone of the passenger with the given ID
EXPORT void get_passenger_info(const char* id, char* out_name, char* out_mail, char* out_phone);

// --- Seats ---
// Returns the number of available seats of the given class (business/economy) on the flight
EXPORT int  get_available_seat_count(int flightID, int isBusinessClass);
// Fills out_seat with the seat label (e.g. "3B") at the given index for the given flight and class
EXPORT void get_seat_by_index(int flightID, int isBusinessClass, int index, char* out_seat);

// --- File Handling ---
// Persists all current in-memory system data to CSV files on disk
EXPORT void save_all_data();
// Loads all previously saved CSV data back into memory, restoring system state
EXPORT void load_all_data();

// --- SuperAdmin ---
// Creates a new Admin account; returns 1 on success, 0 on failure
EXPORT int add_admin(const char* name, const char* id, const char* phone, const char* email);
// Removes the Admin with the given national ID; returns 1 on success, 0 on failure
EXPORT int remove_admin(const char* id);

#ifdef __cplusplus
}
#endif

#endif
