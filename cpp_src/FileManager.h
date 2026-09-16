#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include <fstream>          // For ofstream/ifstream (file I/O)
#include <iostream>         // For std::cout (status messages during save/load)
#include "Passenger.h"
#include "Admin.h"
#include "SuperAdmin.h"
#include "Flight.h"
#include "Ticket.h"
#include "SystemManager.h"

using namespace std;

// FileManager is a purely static utility class
// It handles all CSV-based persistence (save, load, clear) and provides
// reporting helpers that print summaries to stdout
// There is no instance state; every method is called on the class itself
class FileManager {
public:
    // --- File Saving  ---
    // Writes all passengers to the specified CSV file (one row per passenger)
    static void savePassengersToCSV(const vector<Passenger*>& passengers, const string& filename);
    // Writes all flights to the specified CSV file (one row per flight)
    static void saveFlightsToCSV(const vector<Flight*>& flights, const string& filename);
    // Writes all tickets to the specified CSV file (one row per ticket, with totals)
    static void saveTicketsToCSV(const vector<Ticket*>& tickets, const string& filename);
    // Writes all admin accounts to the specified CSV file (one row per admin)
    static void saveAdminsToCSV(const vector<Admin*>& admins, const string& filename);
    // Writes all super admin accounts to the specified CSV file (one row per super admin)
    static void saveSuperAdminsToCSV(const vector<SuperAdmin*>& superAdmins, const string& filename);
    // Writes every individual piece of luggage (across all tickets) to the specified CSV file
    static void saveLuggageToCSV(const vector<Ticket*>& tickets, const string& filename);

    // --- Master Save Method ---
    // Calls all individual save functions in one go using hard-coded file paths
    static void saveAllSystemData(SystemManager* sys);

    // --- Master Load Method ---
    // Reads all CSV files and reconstructs the full system state in memory
    static void loadAllSystemData(SystemManager* sys);

    // --- Reporting Functions ---
    // Prints a full history (profile + tickets) for the passenger with the given national ID
    static void generatePassengerReport(SystemManager* sys, const string& searchID);
    // Prints a high-level summary: total passengers, flights, tickets, and admins
    static void generateSystemSummaryReport(SystemManager* sys);

    // Utility
    // Resets all CSV files to header-only (empty data rows), effectively wiping persisted data
    static void clearAllFiles();
};

#endif   // End of include guard for FILEMANAGER_H
