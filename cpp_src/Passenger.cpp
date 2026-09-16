#include "Passenger.h"
#include "SystemManager.h"
using namespace std;

// Constructor: calls the User base constructor for the four identity fields,
// then stores the passenger-specific password.
Passenger::Passenger(const string& name, const string& id, const string& phone, const string& email, const string& pass)
    : User(name, id, phone, email), password(pass) {}

// Returns the string "Passenger" to identify this user's type polymorphically.
string Passenger::getType() const {
    return "Passenger";
}

// Prints all four identity fields to stdout (no access guard — passengers always have access)
void Passenger::displayInfo() const {
    cout << "Name        : " << name        << "\n"   // Full name inherited from User
         << "National ID : " << nationalID  << "\n"   // National ID inherited from User
         << "Phone       : " << phoneNumber << "\n"   // Phone number inherited from User
         << "Email       : " << email       << "\n";  // Email address inherited from User
}

// Returns the stored password (used by AuthSystem::passengerLogin to validate credentials)
string Passenger::getPassword() const {
    return password;
}

// Returns a copy of the myTickets vector (caller gets a snapshot, not a reference)
vector<Ticket*> Passenger::getTickets() const {
    return myTickets;
}

// Removes the ticket with the given ID from this passenger's personal ticket list
// Uses erase-remove idiom with a lambda to find the matching ticket by ID
void Passenger::removeTicket(int ticketID) {
    myTickets.erase(
        remove_if(myTickets.begin(), myTickets.end(),
            [ticketID](Ticket* t){ return t->getTicketID() == ticketID; }),  // Lambda: true if IDs match
        myTickets.end()   // Erase everything from the new logical end to the physical end
    );
}

// --- Ticket management ---

// Appends the given ticket pointer to myTickets
// Guards against null pointers before pushing
void Passenger::addTicket(Ticket* t) {
    if (t) myTickets.push_back(t);   // Only add if the pointer is non-null
}

// Prints all tickets owned by this passenger using the Ticket << operator
// If no tickets exist, prints a friendly message instead
void Passenger::viewTickets() const {
    if (myTickets.empty()) {
        cout << "You have no tickets.\n";   // No tickets registered for this passenger
        return;
    }
    cout << "=== Your Tickets ===\n";
    for (auto t : myTickets)
        cout << *t;   // Calls Ticket::operator<< for each ticket (full details including luggage)
}

// --- Luggage overview ---

// Prints all luggage across all tickets by printing each full ticket (which includes luggage)
// If no tickets exist, prints a message
void Passenger::viewAllLuggage() const {
    if (myTickets.empty()) {
        cout << "No tickets — no luggage.\n";   // Cannot have luggage without a ticket
        return;
    }
    cout << "=== Your Luggage ===\n";
    for (auto t : myTickets)
        cout << *t;   // Ticket's << operator already includes luggage details
}

// --- Getters ---

// Mirror getters that expose User's protected fields with passenger-specific naming
string Passenger::getPassengerName()  const { return name;        }   // Returns inherited name field
string Passenger::getPassengerID()    const { return nationalID;  }   // Returns inherited nationalID field
string Passenger::getPassengerEmail() const { return email;       }   // Returns inherited email field
string Passenger::getPassengerPhone() const { return phoneNumber; }   // Returns inherited phoneNumber field

// --- Setters ---

// Updates the passenger's name field directly (inherited protected member)
void Passenger::setPassengerName(const string& n)  { name        = n; }
// Updates the passenger's email field directly (inherited protected member)
void Passenger::setPassengerEmail(const string& e) { email       = e; }
// Updates the passenger's phone number field directly (inherited protected member)
void Passenger::setPassengerPhone(const string& p) { phoneNumber = p; }
