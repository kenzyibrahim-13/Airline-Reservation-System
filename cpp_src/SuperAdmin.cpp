#include "SuperAdmin.h"
#include "SystemManager.h"
#include "FileManager.h"

using namespace std;

// Definition of the static shared super-admin password
// Different from the regular Admin master password to enforce role separation
const string SuperAdmin::SUPER_PASSWORD = "SuperPass_9999";

// Constructor: initialises User identity fields and sets access to false by default
// Access is only granted after a successful call to SuperAdmin::login()
SuperAdmin::SuperAdmin(const string& name, const string& id, const string& phone, const string& email)
    : User(name, id, phone, email), access(false) {}

// Static login — verifies the SuperAdmin ID exists in the system,
// then checks the super password (different from the regular admin password)
SuperAdmin* SuperAdmin::login(const string& id, const string& pass) {
    SystemManager* sys = SystemManager::getInstance();    // Get the singleton system manager
    User* found = sys->searchSuperAdminID(id);            // Look for a SuperAdmin with the given ID

    if (!found) {
        cout << "No SuperAdmin account found with ID: " << id << "\n";  // ID not registered
        return nullptr;   // Login fails; return null
    }

    SuperAdmin* sa = dynamic_cast<SuperAdmin*>(found);   // Safely downcast User* to SuperAdmin*
    if (!sa) return nullptr;                             // Cast failed (shouldn't happen, just incase)

    if (pass == SUPER_PASSWORD) {             // Check supplied password against the stored secret
        sa->access = true;                    // Grant access on successful password match
        cout << "SuperAdmin login successful. Welcome, " << sa->getName() << "!\n";
        return sa;                            // Return the authenticated SuperAdmin pointer
    }

    return nullptr;   // Wrong password; return null to signal failure
}

// Returns the string "SuperAdmin" to identify this user's type polymorphically
string SuperAdmin::getType() const {
    return "SuperAdmin";
}

// Prints the SuperAdmin's identity fields to stdout.
// Blocked if access is false (login has not been completed)
void SuperAdmin::displayInfo() const {
    if (!access) { cout << "Access denied.\n"; return; }   // Guard: deny if not logged in
    cout << "Name        : " << name        << "\n"        // Print full name (inherited from User)
         << "National ID : " << nationalID  << "\n"        // Print national ID (inherited from User)
         << "Phone       : " << phoneNumber << "\n"        // Print phone number (inherited from User)
         << "Email       : " << email       << "\n";       // Print email address (inherited from User)
}

// Returns the current value of the private access flag
bool SuperAdmin::hasAccess() const {
    return access;
}

// Create a new Admin and add them to the system
// SuperAdmin verifies the action is authorised; the Admin object itself
// is created with access=false (they still need to call Admin::login() to operate)
bool SuperAdmin::addAdmin(const string& name, const string& id,const string& phone, const string& email) {
    if (!access) { cout << "Access denied.\n"; return false; }   // Guard: require login

    if(User::build(UserType:: ADMIN,name,id,phone,email)){ return true; }   // Build the Admin via factory; return true on success
    return false;   // Build failed (e.g. duplicate ID)
}

// Remove an Admin account from the system entirely
bool SuperAdmin::removeAdmin(const string& adminID) {
    if (!access) { cout << "Access denied.\n"; return false; }   // Guard: require login

    SystemManager* sys = SystemManager::getInstance();   // Get the singleton
    if (sys->removeAdmin(adminID)) {                     // Ask SystemManager to remove the admin
        cout << "Admin " << adminID << " removed from the system.\n";
        return true;   // Removal succeeded
    }
    cout << "Admin " << adminID << " not found.\n";
    return false;   // Admin ID was not found in the system
}

// Delegates to SystemManager to print all currently registered admins
// Blocked if access is false.
void SuperAdmin::displayAllAdmins() const {
    if (!access) { cout << "Access denied.\n"; return; }   // Guard: require login
    SystemManager::getInstance()->displayAllAdmins();       // Let the singleton handle the output
}

//Delete all files data
// Performs a full factory reset: clears all CSV data files and exits the program
// Requires explicit confirmation from the user before proceeding
void SuperAdmin::resetSystem() const {
    if (!access) {
        cout << "Access denied. Cannot reset system.\n";   // Guard: require login
        return;
    }

    char confirm;
    cout << "\n!!! WARNING !!!\n";
    cout << "You are about to delete ALL saved data (Passengers, Flights, Tickets, Admins).\n";
    cout << "Are you absolutely sure? (y/n): ";
    cin >> confirm;   // Read a single character confirmation from the user

    if (confirm == 'y' || confirm == 'Y') {     // Proceed only on explicit 'y' or 'Y'
        FileManager::clearAllFiles();           // Wipe all CSV data files (headers preserved)
        cout << "Factory Reset Complete. Program exiting to prevent data overlap.\n";
        exit(0);   // Exit immediately to prevent stale in-memory data from being re-saved
    } else {
        cout << "Reset cancelled.\n";   // User chose not to proceed
    }
}
