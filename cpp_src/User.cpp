#include "User.h"
#include "SystemManager.h"
#include "Passenger.h"
#include "Admin.h"
using namespace std;

// Constructor: initialises all four inherited identity fields via member-initialiser list
User::User(const string& newName, const string& newID,const string& userPhone, const string& userEmail)
    : name(newName), nationalID(newID),phoneNumber(userPhone), email(userEmail) {}

// Returns the user's full name
string User::getName() const {
    return name;
}

// Returns the user's national ID (primary key used for login and system lookups)
string User::getNationalID() const {
    return nationalID;
}

// Returns the user's email address
string User::getEmail() const {
    return email;
}

// Returns the user's phone number
string User::getPhoneNumber() const {
    return phoneNumber;
}

// Updates the user's email address to the supplied value
void User::setEmail(const string& newEmail) {
    email = newEmail;
}

// Updates the user's phone number to the supplied value
void User::setPhoneNumber(const string& newPhone) {
    phoneNumber = newPhone;
}

// Static factory method: creates a new Passenger or Admin based on the requested role
// Checks for duplicate national IDs before creating anything
// Registers the new object in SystemManager and returns it
// Returns nullptr if the role is unrecognised or the ID is already taken
User* User::build(UserType role, const string& name, const string& id,const string& phone, const string& email,const string& password) {
    if (role == PASSENGER){                                  // Handle Passenger creation
            SystemManager* sys = SystemManager::getInstance();   // Get the singleton
            if (sys->searchUserID(id)) {                         // Check if this ID is already registered
                    cout << "Error: ID " << id << " is already registered.\n";
                    return nullptr;                               // Reject duplicate; return null
                    }
            Passenger* p = new Passenger(name, id, phone, email, password);   // Create the Passenger
            sys->addUser(p);                                                   // Register in system
            cout << " Passenger created: " << name << " (ID: " << id << ")\n";
            return p;                                                          // Return the new Passenger
            }
    else if (role == ADMIN){                                 // Handle Admin creation
            SystemManager* sys = SystemManager::getInstance();   // Get the singleton
            if (sys->searchUserID(id)) {                         // Check if this ID is already registered
                    cout << "Error: ID " << id << " is already registered.\n";
                    return nullptr;                               // Reject duplicate; return null
                    }
            Admin* a = new Admin(name, id, phone, email);   // Create the Admin (no password needed)
            sys->addUser(a);                                 // Register in system
            cout << "Admin created: " << name << " (ID: " << id << ")\n";
            return a;                                        // Return the new Admin
            }
            else
                return nullptr;   // Unknown role; return null
}

// Compares two User objects by national ID for equality.
bool User::operator==(const User& other) const {
    return nationalID == other.nationalID;   // Equal if and only if national IDs match
}

// Prints a one-line summary of any user using polymorphism (getType() resolves to the subclass)
// Format: "[Type] Name | ID: nationalID"
ostream& operator<<(ostream& out, const User& user) {
    out << "[" << user.getType() << "] "<< user.name << " | ID: " << user.nationalID;
    return out;   // Return the stream to allow chaining
}
