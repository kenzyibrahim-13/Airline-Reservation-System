#include "Admin.h"
#include "Passenger.h"
#include "SystemManager.h"
using namespace std;


// SuperAdmin calls this constructor after verifying the password itself
// Initialises the User base with the four identity fields, and sets access to false by default
Admin::Admin(const string& name, const string& id,const string& phone, const string& email)
    : User(name, id, phone, email), access(false) {}   // access starts as false until a successful login grants it


// --- Overrides ---

// Returns a plain string identifying this object as an "Admin" (used in polymorphic type checks)
string Admin::getType() const {
    return "Admin";
}

// Prints the admin's identity fields to stdout
// If access is false the operation is blocked and an error message is shown instead.
void Admin::displayInfo() const {
    if (!access) { cout << "Access denied.\n"; return; }   // Guard: only show info to authorised admins
    cout << "Name        : " << name        << "\n"        // Print full name inherited from User
         << "National ID : " << nationalID  << "\n"        // Print national ID inherited from User
         << "Phone       : " << phoneNumber << "\n"        // Print phone number inherited from User
         << "Email       : " << email       << "\n";       // Print email address inherited from User
}

// Simple getter for the private access flag
// Used by other components to determine whether this Admin may perform operations
bool Admin::hasAccess() const {
    return access;
}



// --- Reporting ---

// Lists all flights currently registered in the system
// Delegates the actual work to SystemManager; blocked if the admin lacks access
void Admin::displayAllFlights() const {
    if (!access) { cout << "Access denied.\n"; return; }   // Guard: prevent unauthorised use
    SystemManager::getInstance()->displayAllFlights();      // Ask the singleton to print all flights
}

// Lists all users (passengers, admins, etc.) currently registered in the system
// Delegates the actual work to SystemManager; blocked if the admin lacks access
void Admin::displayAllUsers() const {
    if (!access) { cout << "Access denied.\n"; return; }   // Guard: prevent unauthorised use
    SystemManager::getInstance()->displayAllUsers();        // Ask the singleton to print all users
}
