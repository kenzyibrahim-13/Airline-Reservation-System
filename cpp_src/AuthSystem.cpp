#include "AuthSystem.h"

// Constructor: fetches the global SystemManager singleton and stores it for later queries
AuthSystem::AuthSystem() {
    sys = SystemManager::getInstance();   // Retrieve (or create) the single SystemManager instance
}

// --- PASSENGER SIGN UP ---
// Creates a new Passenger account using the User factory method
// Returns true if the account was successfully created, false otherwise (e.g. duplicate ID)
bool AuthSystem::passengerSignup(string name, string id, string phone, string email, string pass) {

    if (User::build(UserType::PASSENGER, name, id, phone, email, pass)) {   // Delegate creation to the User factory
        return true;    // Account created successfully
    }
    return false;       // Creation failed (duplicate ID or invalid data)
}

// --- PASSENGER LOGIN ---
// Looks up the passenger by national ID and checks if the supplied password matches
// Returns a pointer to the Passenger on success, or nullptr if ID not found or password wrong
Passenger* AuthSystem::passengerLogin(string id, string pass) {

    Passenger* p = sys->searchPassengerID(id);   // Search for a passenger with the given national ID

    if (p != nullptr && p->getPassword() == pass) {   // Passenger found and passwords match
        return p;    // Authentication successful; return the passenger pointer
    }

    return nullptr;   // Authentication failed; return null to signal failure
}

// --- ADMIN LOGIN ---
// First validates the supplied password against the single shared admin master password
// If it matches, looks up the Admin by national ID in the system
// Returns a pointer to the Admin on success, or nullptr if password wrong or ID not found
Admin* AuthSystem::adminLogin(string id, string pass) {

    const string MASTER_PASS = "PassCode_1230";   // Hard-coded shared password for all admin accounts

    if (pass != MASTER_PASS) {    // Reject immediately if the master password does not match
        return nullptr;           // Wrong password; deny access
    }

    // Password is correct; find and return the admin with the given ID (cast from User* to Admin*)
    return dynamic_cast<Admin*>(sys->searchAdminID(id));
}
