#ifndef SUPERADMIN_H
#define SUPERADMIN_H

#include "User.h"      // SuperAdmin inherits from User (identity fields + factory method)
#include "Admin.h"
#include <string>
#include <iostream>
using namespace std;

// SuperAdmin is the top-level authority in the system
// Its ONLY responsibilities are:
//   1. addAdmin()    — create a new Admin account in the system
//   2. removeAdmin() — delete an Admin account from the system
//
// SuperAdmin cannot book tickets, cancel flights, or touch passengers
// That is intentional: separation of concerns between account management
// (SuperAdmin) and operational management (Admin)
//
// There should be exactly one SuperAdmin — enforced via a private password
// that is different from the regular Admin password
class SuperAdmin : public User {   // SuperAdmin is a concrete subclass of User
private:
    bool access;                        // true only after a successful login via SuperAdmin::login()
    static const string SUPER_PASSWORD; // Shared secret used to authenticate the SuperAdmin; defined in .cpp

public:
    // Constructor: initialises User fields; access starts as false until login() succeeds
    SuperAdmin(const string& name, const string& id, const string& phone, const string& email);

    // Login: looks up the SuperAdmin by ID in the system, then verifies the super password
    // Sets access=true and returns a pointer to the SuperAdmin on success; returns nullptr on failure
    static SuperAdmin* login(const string& id, const string& pass);

    // --- Overrides from User ---
    string getType()     const override;   // Returns the string "SuperAdmin"
    void   displayInfo() const override;   // Prints identity fields; blocked if access == false

    // Returns the current value of the access flag
    bool hasAccess() const;

    // --- Core responsibilities ---
    // Create a new Admin account and register it in the system
    // Returns true on success, false if access is denied or the ID already exists
    bool addAdmin(const string& name, const string& id, const string& phone, const string& email);

    // Remove an Admin account from the system by national ID
    // Returns true on success, false if access is denied or the admin is not found
    bool removeAdmin(const string& adminID);

    // Prints a list of all admins currently registered in the system
    void displayAllAdmins() const;

    // Performs a factory reset: wipes all CSV data files after confirmation
    // Exits the program after reset to prevent data overlap
    void resetSystem() const;
};

#endif
