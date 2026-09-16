#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <vector>
#include <iostream>
#include "User.h"
#include "Flight.h"
#include "Ticket.h"
#include "Type.h"
using namespace std;


class Admin;       // Forward declaration: avoids including Admin.h
class Passenger;   // Forward declaration: avoids circular include with Passenger.h
class SuperAdmin;  // Forward declaration: avoids circular include with SuperAdmin.h
class User;

// SystemManager is a Singleton — only one instance ever exists
// It is the central data store for the entire system:
//   - All users (passengers, admins, superadmin) stored in allUsers
//   - Typed sub-vectors (allPassengers, allAdmins) for fast lookup
//   - All flights and tickets
//
// Design pattern: Singleton
class SystemManager {
private:
    int nextTicketID = 1;              // Auto-incrementing counter; used by bookTicket() to assign unique IDs
    static SystemManager* instance;   // The single shared instance (null until first call to getInstance())
    SystemManager() {}                // Private constructor prevents direct instantiation from outside

    // All collections store raw (non-owning) pointers; SystemManager owns and frees these in ~SystemManager()
    vector<User*>       allUsers;         // Every user regardless of type (authoritative ownership)
    vector<Passenger*>  allPassengers;    // Subset of allUsers containing only Passenger objects
    vector<Admin*>      allAdmins;        // Subset of allUsers containing only Admin objects
    vector<SuperAdmin*> allSuperAdmins;   // Subset of allUsers containing only SuperAdmin objects
    vector<Flight*>     allFlights;       // All registered flights
    vector<Ticket*>     allTickets;       // All booked tickets

public:
    // Prevent copying — Singleton must be unique
    SystemManager(const SystemManager&)            = delete;   // Copy constructor disabled
    SystemManager& operator=(const SystemManager&) = delete;   // Copy assignment disabled

    // --- Getters (return references so callers can iterate without copying) ---
    vector<Flight*>&    getAllFlights()     ;   // Returns reference to the flights vector
    vector<Passenger*>& getAllPassengers()  ;   // Returns reference to the passengers vector
    vector<Ticket*>&    getAllTickets()     ;   // Returns reference to the tickets vector
    vector<Admin*>&      getAllAdmins()      ;  // Returns reference to the admins vector
    vector<SuperAdmin*>& getAllSuperAdmins() ;  // Returns reference to the super admins vector

    // The only way to get the instance; creates it on first call
    static SystemManager* getInstance();

    // --- Search (linear scans; return nullptr if not found) ---
    Flight*     searchFlightID(int id);                   // Find a flight by numeric ID
    User*       searchUserID(const string& id);           // Find any user by national ID
    Passenger*  searchPassengerID(const string& id);      // Find a passenger by national ID
    User*       searchAdminID(const string& id);          // Find an admin by national ID (returns User*)
    User*       searchSuperAdminID(const string& id);     // Find a super admin by national ID (returns User*)
    Ticket*     searchTicketID(int id);                   // Find a ticket by numeric ID

    // --- Add ---
    void addUser(User* u);       // Routes to the correct sub-vector (allPassengers / allAdmins / allSuperAdmins) automatically
    void addFlight(Flight* f);   // Appends a flight to allFlights
    void addTicket(Ticket* t);   // Appends a ticket to allTickets
    // Scans allTickets and advances nextTicketID past the highest existing ID (called after loading from CSV).
    void syncNextTicketID();

    // --- Remove ---
    bool removeFlight(int flightID);          // Deletes and removes a flight by ID
    bool removeTicket(int ticketID);          // Cancels a ticket: frees the seat, unlinks from passenger, deletes
    bool removeAdmin(const string& adminID);  // Removes an admin from both allAdmins and allUsers
    bool cancelFlight(int flightID);          // Cancels all tickets on a flight, then removes the flight

    // --- Booking (core transaction) ---
    // Creates a Ticket, books the seat on the Flight, attaches to the Passenger
    // Returns the new Ticket on success, or nullptr on any validation failure
    Ticket* bookTicket(const string& passengerID, int flightID, const string& seat, TicketType type);

    // --- Display / Reporting ---
    void displayAllUsers()      const;   // Prints a one-line summary of every user
    void displayAllFlights()    const;   // Prints full details of every flight
    void displayAllAdmins()     const;   // Prints a one-line summary of every admin
    void displayAllPassengers() const;   // Prints a one-line summary of every passenger

    // Destructor: frees all dynamically allocated users, flights, and tickets
    ~SystemManager();
};

#endif
