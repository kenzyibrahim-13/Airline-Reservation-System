#ifndef PASSENGER_H
#define PASSENGER_H

#include "User.h"     // Passenger inherits from User (identity fields + factory method)
#include "Ticket.h"   // Passenger owns a collection of Ticket pointers
#include <vector>
using namespace std;

// Represents a passenger (customer) in the system
// Passengers can sign up, log in, book/cancel tickets, and view luggage
// Authentication is done via nationalID + password
class Passenger : public User {   // Passenger is a concrete subclass of User
private:
    string          password;   // hashed or plain password for login
    vector<Ticket*> myTickets;  // tickets owned by this passenger (non-owning borrowed pointers)

public:
    // Constructor: initialises all User fields plus the passenger-specific password
    Passenger(const string& name, const string& id,
              const string& phone, const string& email,
              const string& password);

    // --- Overrides from User ---
    string getType()        const override;   // Returns the string "Passenger"
    void   displayInfo()    const override;   // Prints identity fields to stdout

    // Password accessor (used by AuthSystem to validate login credentials)
    string getPassword() const;

    // --- Ticket management ---
    void addTicket(Ticket* t);           // Appends a ticket pointer to myTickets (called after booking)
    void viewTickets() const;            // Prints a summary of all tickets owned by this passenger
    void removeTicket(int ticketID);     // Removes the ticket with the given ID from myTickets

    // --- Luggage overview ---
    void viewAllLuggage() const;         // Prints luggage details across all of this passenger's tickets

    // --- Getters (expose protected User fields cleanly for external use) ---
    string getPassengerName()  const;    // Returns name (mirrors User::getName())
    string getPassengerID()    const;    // Returns nationalID (mirrors User::getNationalID())
    string getPassengerEmail() const;    // Returns email (mirrors User::getEmail())
    string getPassengerPhone() const;    // Returns phoneNumber (mirrors User::getPhoneNumber())
    vector<Ticket*> getTickets() const;  // Returns a copy of the myTickets vector

    // --- Setters ---
    void setPassengerName(const string& n);    // Updates name field
    void setPassengerEmail(const string& e);   // Updates email field
    void setPassengerPhone(const string& p);   // Updates phoneNumber field
};

#endif
