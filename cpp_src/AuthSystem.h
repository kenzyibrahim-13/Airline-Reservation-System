#ifndef AUTHSYSTEM_H
#define AUTHSYSTEM_H

#include <string>
#include "SystemManager.h"
#include "Passenger.h"
#include "Admin.h"
#include "User.h"

using namespace std;

// AuthSystem centralises all login and sign-up logic
// It acts as the entry point for passengers, admins, and the super admin
// to authenticate before they can interact with the rest of the system
class AuthSystem {
private:
    SystemManager* sys;   // Pointer to the singleton SystemManager used for all user lookups

public:

    // Default constructor; fetches and stores the SystemManager singleton instance
    AuthSystem();

    // Attempts to create a new Passenger account with the supplied credentials
    // Returns true on success, false if the ID already exists or validation fails
    bool passengerSignup(string name, string id, string phone, string email, string pass);

    // Looks up a passenger by ID and validates their password
    // Returns a pointer to the Passenger on success, or nullptr on failure
    Passenger* passengerLogin(string id, string pass);

    // Validates the supplied password against the hard-coded admin master password,
    // then looks up the admin by ID
    // Returns a pointer to the Admin on success, or nullptr on failure
    Admin* adminLogin(string id, string pass);
};

#endif
