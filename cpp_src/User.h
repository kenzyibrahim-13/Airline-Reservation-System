#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>

#include "Type.h"
using namespace std;

class Passenger;      // Forward declaration: avoids circular include with Passenger.h
class SystemManager;  // Forward declaration: avoids circular include with SystemManager.h

// Abstract base class for all users in the system
// Derived classes: Passenger, Admin
// Holds the four common identity fields and declares the two pure virtual methods
// that every derived class must implement: displayInfo() and getType()
class User {
protected:
    // Protected so derived classes (Passenger, Admin, SuperAdmin) can access them directly
    string name;          // Full name of the user
    string nationalID;    // Unique national ID used for login and lookups
    string phoneNumber;   // Contact phone number
    string email;         // Contact email address

public:
    // Constructor: initialises all four identity fields
    User(const string& newName, const string& newID,
         const string& userPhone, const string& userEmail);

    // Virtual destructor: ensures correct cleanup when deleting via a User* pointer
    virtual ~User() {}

    // Pure virtual functions — every derived class must implement these
    virtual void displayInfo() const = 0;     // Print the user's full details to stdout
    virtual string getType()   const = 0;     // Return a string identifying the subclass type

    // Getters — all const because they do not modify the object
    string getName()        const;   // Returns the user's full name
    string getNationalID()  const;   // Returns the user's national ID
    string getEmail()       const;   // Returns the user's email address
    string getPhoneNumber() const;   // Returns the user's phone number

    // Static factory method: creates either a Passenger or an Admin based on role,
    // registers them in SystemManager, and returns the new object
    // Returns nullptr if the ID already exists or the role is unrecognised
    static User* build(UserType role, const string& name, const string& id,
                   const string& phone, const string& email,
                   const string& password = "");   // password defaults to "" for Admin (no password needed)

    // Setters — allow in-place updates of mutable contact fields
    void setEmail(const string& newEmail);          // Updates the email address
    void setPhoneNumber(const string& newPhone);    // Updates the phone number

    // Operator overloading: compare two users by national ID (equality check)
    bool operator==(const User& other) const;

    // Operator overloading: print a one-line summary of any user to an output stream
    friend ostream& operator<<(ostream& out, const User& user);
};

#endif
