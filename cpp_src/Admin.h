#ifndef ADMIN_H          // Include guard: prevents this header from being included more than once
#define ADMIN_H

#include "User.h"        // Inherits from User base class
#include "Type.h"        // Includes shared enums and type definitions (e.g. UserType)
#include <string>
#include <iostream>      // For std::cout (used in displayInfo)
using namespace std;

// Admin is created only by SuperAdmin
// Once created, Admin logs in with their national ID + the shared admin password
// An Admin with access=true can perform all operational tasks in the system
class Admin : public User {   // Admin inherits publicly from User
private:
    bool access;  // true only after a successful login or creation by SuperAdmin

public:
    // Constructor: called by SuperAdmin::addAdmin() after password verification
    Admin(const string& name, const string& id,
          const string& phone, const string& email);


    // --- Overrides from User ---
    string getType()     const override;   // Returns the string "Admin" to identify this user type
    void   displayInfo() const override;   // Prints admin details; blocked if access == false

    bool hasAccess() const;  // used to guard all operations; returns the value of the access flag


    // --- Reporting ---
    void displayAllFlights() const;   // Delegates to SystemManager to list all flights; requires access
    void displayAllUsers()   const;   // Delegates to SystemManager to list all users; requires access
};

#endif
