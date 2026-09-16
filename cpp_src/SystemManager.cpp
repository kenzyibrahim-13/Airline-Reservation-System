#include "SystemManager.h"
#include "Admin.h"
#include "Passenger.h"
#include "SuperAdmin.h"
#include "Ticket.h"
using namespace std;

// Initialise the static singleton pointer to null; instance is created on first getInstance() call
SystemManager* SystemManager::instance = nullptr;

// Returns the single shared SystemManager instance
// Creates it on the first call (lazy initialisation / Singleton pattern)
SystemManager* SystemManager::getInstance() {
    if (!instance)
        instance = new SystemManager();   // First call: construct the singleton
    return instance;
}

// --- Collection getters (return non-const references for external iteration) ---

vector<Flight*>& SystemManager::getAllFlights()  {
    return allFlights;      // Caller can iterate or modify the flights vector directly
}

vector<Passenger*>& SystemManager::getAllPassengers()  {
    return allPassengers;   // Caller can iterate or modify the passengers vector directly
}
vector<Ticket*>& SystemManager::getAllTickets() {
    return allTickets;      // Caller can iterate or modify the tickets vector directly
}
vector<Admin*>& SystemManager::getAllAdmins() {
    return allAdmins;       // Caller can iterate or modify the admins vector directly
}
vector<SuperAdmin*>& SystemManager::getAllSuperAdmins() {
    return allSuperAdmins;  // Caller can iterate or modify the super admins vector directly
}

// --- Search ---

// Linear scan of allFlights; returns the first Flight with a matching ID, or nullptr
Flight* SystemManager::searchFlightID(int id) {
    for (auto f : allFlights)
        if (f->getFlightID() == id) return f;   // Match found; return immediately
    return nullptr;   // No flight with this ID exists
}

// Linear scan of allUsers; returns the first User with a matching national ID, or nullptr
User* SystemManager::searchUserID(const string& id) {
    for (auto u : allUsers)
        if (u->getNationalID() == id) return u;   // Match found
    return nullptr;   // Not found
}

// Linear scan of allPassengers; returns the Passenger with the matching national ID, or nullptr
Passenger* SystemManager::searchPassengerID(const string& id) {
    for (auto p : allPassengers)
        if (p->getNationalID() == id) return p;   // Match found
    return nullptr;   // Not found
}

// Linear scan of allAdmins; returns the Admin (as User*) with the matching national ID, or nullptr
User* SystemManager::searchAdminID(const string& id) {
    for (auto a : allAdmins)
        if (a->getNationalID() == id) return a;   // Match found
    return nullptr;   // Not found
}

// Linear scan of allSuperAdmins; returns the SuperAdmin (as User*) with the matching ID, or nullptr
User* SystemManager::searchSuperAdminID(const string& id) {
    for (auto sa : allSuperAdmins)
        if (sa->getNationalID() == id) return sa;   // Match found
    return nullptr;   // Not found
}

// Linear scan of allTickets; returns the Ticket with the matching numeric ID, or nullptr
Ticket* SystemManager::searchTicketID(int id) {
    for (auto t : allTickets)
        if (t->getTicketID() == id) return t;   // Match found
    return nullptr;   // Not found
}

// --- Add ---

// Adds a user to allUsers and also to the appropriate typed sub-vector
// Uses getType() and dynamic_cast to route correctly without relying on explicit type checks elsewhere
void SystemManager::addUser(User* u) {
    if (!u) return;          // Guard against null pointers
    allUsers.push_back(u);   // Every user goes into the master list regardless of type

    const string type = u->getType();                           // Resolve the subclass type string
    if (type == "Passenger")
        allPassengers.push_back(dynamic_cast<Passenger*>(u));  // Add to passengers sub-vector
    else if (type == "Admin")
        allAdmins.push_back(dynamic_cast<Admin*>(u));          // Add to admins sub-vector
    else if (type == "SuperAdmin")
        allSuperAdmins.push_back(dynamic_cast<SuperAdmin*>(u)); // Add to super admins sub-vector
}

// Appends a flight to allFlights; guards against null pointers
void SystemManager::addFlight(Flight* f) {
    if (f) allFlights.push_back(f);
}

// Appends a ticket to allTickets; guards against null pointers
void SystemManager::addTicket(Ticket* t) {
    if (t) allTickets.push_back(t);
}

// --- Remove ---

// Finds the flight with the given ID, deletes it, and erases it from allFlights
// Returns true if the flight was found and removed, false otherwise
bool SystemManager::removeFlight(int id) {

    // Remove the flight itself
    for (auto it = allFlights.begin(); it != allFlights.end(); ++it) {

        if ((*it)->getFlightID() == id) {
            delete *it;              // Free the Flight object's memory
            allFlights.erase(it);   // Remove the dangling pointer from the vector
            return true;
        }
    }
    return false;   // No flight with this ID was found
}

// Cancels a flight: first cancels all tickets associated with it, then removes the flight itself.
// Returns true if the flight existed and was removed (even if it had no tickets).
bool SystemManager::cancelFlight(int flightID) {
    vector<int> ticketsToCancel;
    for (auto t : allTickets)
        if (t->getFlightID() == flightID)
            ticketsToCancel.push_back(t->getTicketID());   // Collect all matching ticket IDs first

    for (int id : ticketsToCancel)
        removeTicket(id);   // Cancel each collected ticket (frees seats and unlinks from passengers)

    return removeFlight(flightID);   // Finally remove the flight itself
}

// Cancels a single ticket: frees the associated seat, unlinks from the passenger, deletes, and erases
// Returns true if the ticket was found and removed, false otherwise
bool SystemManager::removeTicket(int id) {
    for (auto it = allTickets.begin(); it != allTickets.end(); ++it) {
        if ((*it)->getTicketID() == id) {
            Flight* f = searchFlightID((*it)->getFlightID());     // Find the associated flight
            if(f) f -> cancelTakenSeat((*it)->getSeatNumber());   // Mark the seat as available again

            Passenger *p = SystemManager::getInstance()->searchPassengerID((*it)->getPassengerID());
            if (p) p->removeTicket(id);   // Unlink the ticket from the passenger's personal list

            delete *it;               // Free the Ticket object's memory
            allTickets.erase(it);     // Remove the dangling pointer from the vector
            return true;
        }
    }
    return false;   // No ticket with this ID was found
}

// Remove an Admin from both allAdmins and allUsers vectors
bool SystemManager::removeAdmin(const string& id) {
    // Remove from allAdmins (erase-by-iterator; no delete here — memory freed via allUsers below)
    for (auto it = allAdmins.begin(); it != allAdmins.end(); ++it) {
        if ((*it)->getNationalID() == id) {
            allAdmins.erase(it);   // Remove from the typed sub-vector (pointer itself not deleted yet)
            break;
        }
    }
    // Remove from allUsers and free memory
    for (auto it = allUsers.begin(); it != allUsers.end(); ++it) {
        if ((*it)->getNationalID() == id && (*it)->getType() == "Admin") {
            delete *it;            // Free the Admin object's memory (only once, here)
            allUsers.erase(it);    // Remove the dangling pointer from the master list
            return true;
        }
    }
    return false;   // Admin was not found in allUsers
}

// --- Booking ---

// Full booking transaction:
//   1. Validate passenger and flight exist
//   2. Book the seat on the flight
//   3. Create a Ticket with a valid Flight*
//   4. Register ticket in system and attach to passenger
Ticket* SystemManager::bookTicket(const string& passengerID, int flightID, const string& seat, TicketType type) {
    string seatCopy = seat;                           // Work with a mutable copy
    Passenger* p = searchPassengerID(passengerID);    // Validate the passenger exists
    if (!p) { cout << "Passenger not found.\n"; return nullptr; }

    Flight* f = searchFlightID(flightID);             // Validate the flight exists
    if (!f) { cout << "Flight not found.\n"; return nullptr; }

    if (!f->bookSpecificSeat(seatCopy, type)) {       // Attempt to reserve the seat on the flight
        cout << "Seat " << seat << " could not be booked"
             << " (full, wrong class, or already taken).\n";
        return nullptr;   // Seat booking failed; return null
    }

    Ticket* t = new Ticket(nextTicketID++, passengerID, flightID, seatCopy, type, f);  // Create ticket; increment counter
    addTicket(t);   // Register the ticket in the global list

    p->addTicket(t);   // Link the ticket to the passenger's personal list

    cout << "Booking successful! Ticket ID: " << t->getTicketID() << "\n";
    return t;   // Return the newly created ticket
}

// --- Display ---

// Prints a one-line summary of every user (all types) using the << operator
void SystemManager::displayAllUsers() const {
    cout << "\n=== All Users in System ===\n";
    for (auto u : allUsers) cout << *u << "\n";   // User::operator<< prints "[Type] Name | ID: ..."
    cout << "===========================\n";
}

// Prints full details of every flight using the Flight << operator
void SystemManager::displayAllFlights() const {
    cout << "\n=== All Flights in System ===\n";
    for (auto f : allFlights) cout << *f << "\n";   // Flight::operator<< prints route, price, capacity
    cout << "=============================\n";
}

// Prints a one-line summary of every admin using the << operator
void SystemManager::displayAllAdmins() const {
    cout << "\n=== All Admins ===\n";
    for (auto a : allAdmins) cout << *a << "\n";   // User::operator<< (polymorphic) prints "[Admin] Name | ID: ..."
    cout << "==================\n";
}

// Prints a one-line summary of every passenger using the << operator
void SystemManager::displayAllPassengers() const {
    cout << "\n=== All Passengers ===\n";
    for (auto p : allPassengers) cout << *p << "\n";   // User::operator<< (polymorphic)
    cout << "======================\n";
}

// Scans allTickets and advances nextTicketID to one past the highest existing ticket ID
// Called after loading data from CSV files to prevent ID collisions on new bookings
void SystemManager::syncNextTicketID() {
    for (auto t : allTickets)
        if (t->getTicketID() >= nextTicketID)
            nextTicketID = t->getTicketID() + 1;   // Keep nextTicketID strictly greater than any loaded ID
}

// --- Destructor ---

// Frees all dynamically allocated memory owned by SystemManager
// Order matters: users first (Passenger::myTickets holds borrowed pointers, not owners),
// then flights, then tickets (SystemManager is the sole owner of Ticket objects)
SystemManager::~SystemManager() {
    // Free all users (Passengers, Admins, SuperAdmins)
    for (auto u : allUsers) delete u;   // Delete each User-derived object
    allUsers.clear();         // Clear the master user list
    allPassengers.clear();    // Clear the typed sub-vector
    allAdmins.clear();        // Clear the typed sub-vector
    allSuperAdmins.clear();   // Clear the typed sub-vector

    // Free all flights
    for (auto f : allFlights) delete f;   // Delete each Flight object
    allFlights.clear();                   // Clear the flights vector

    // Free all tickets
    // Passenger::myTickets stores borrowed (non-owning) pointers
    for (auto t : allTickets) delete t;   // Delete each Ticket object (sole ownership here)
    allTickets.clear();                   // Clear the tickets vector

    instance = nullptr;   // Reset the singleton pointer so getInstance() can recreate if needed
}
