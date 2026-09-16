#include "FileManager.h"
#include "SystemManager.h"
#include <sstream>         // For std::stringstream (used to parse CSV lines)
#include <iostream>


//SECTION 1: SAVING DATA TO FILES

// Writes every registered passenger to a CSV file
// Each row contains: NationalID, Name, Phone, Email, Password
void FileManager::savePassengersToCSV(const vector<Passenger*>& passengers, const string& filename) {
    ofstream outFile(filename);                        // Open (or create) the target file for writing
    if (!outFile.is_open()) return;                    // If the file couldn't be opened, abort silently

    // Header row for Excel columns
    outFile << "NationalID,Name,Phone,Email,Password\n";   // Write the CSV column headers
    for (Passenger* p : passengers) {                       // Iterate over every passenger pointer
        outFile << p->getPassengerID()    << ","            // Column 1: national ID
                << p->getPassengerName()  << ","            // Column 2: full name
                << p->getPassengerPhone() << ","            // Column 3: phone number
                << p->getPassengerEmail() << ","            // Column 4: email address
                << p->getPassword()       << "\n";          // Column 5: password (end the row)
    }
    outFile.close();   // Flush and close the file
}

// Writes every scheduled flight to a CSV file
// Each row contains: FlightID, Origin, Destination, Capacity, Price, Date, Time, BookedSeats
void FileManager::saveFlightsToCSV(const vector<Flight*>& flights, const string& filename) {
    ofstream outFile(filename);              // Open (or create) the target file for writing
    if (!outFile.is_open()) return;          // Abort if the file could not be opened

    outFile << "FlightID,Origin,Destination,Capacity,Price,Date,Time,BookedSeats\n";   // Column headers
    for (Flight* f : flights) {                              // Iterate over every flight pointer
        outFile << f->getFlightID()     << ","               // Column 1: numeric flight ID
                << f->getOrigin()       << ","               // Column 2: origin city/airport
                << f->getDestination()  << ","               // Column 3: destination city/airport
                << f->getCapacity()     << ","               // Column 4: total seat capacity
                << f->getPrice()        << ","               // Column 5: base ticket price
                << f->getDate()         << ","               // Column 6: departure date (YYYY-MM-DD)
                << f->getTime()         << ","               // Column 7: departure time (HH:MM)
                << f->getBookedCount()  << "\n";             // Column 8: number of seats already booked
    }
    outFile.close();   // Flush and close the file
}

// Writes every booked ticket to a CSV file
// Each row contains: TicketID, PassengerID, FlightID, SeatNumber, Class, TotalLuggageWeight, TotalPrice
void FileManager::saveTicketsToCSV(const vector<Ticket*>& tickets, const string& filename) {
    ofstream outFile(filename);              // Open (or create) the target file for writing
    if (!outFile.is_open()) return;          // Abort if the file could not be opened

    outFile << "TicketID,PassengerID,FlightID,SeatNumber,Class,TotalLuggageWeight,TotalPrice\n";   // Column headers
    for (Ticket* t : tickets) {
        // Convert the Enum type to a readable string for Excel
        string ticketClass = (t->getType() == ECONOMY) ? "Economy" : "Business";   // Map enum to human-readable string
        outFile << t->getTicketID()          << ","   // Column 1: unique ticket ID
                << t->getPassengerID()       << ","   // Column 2: owner's national ID
                << t->getFlightID()          << ","   // Column 3: associated flight ID
                << t->getSeatNumber()        << ","   // Column 4: seat label (e.g. "3B")
                << ticketClass               << ","   // Column 5: "Economy" or "Business"
                << t->getTotalWeight()       << ","   // Column 6: combined luggage weight (kg)
                << t->calculateTotalPrice()  << "\n"; // Column 7: total price including extras
    }
    outFile.close();   // Flush and close the file
}

// Writes every admin account to a CSV file
// Each row contains: Role, NationalID, Name, Phone, Email.
void FileManager::saveAdminsToCSV(const vector<Admin*>& admins, const string& filename) {
    ofstream outFile(filename);              // Open (or create) the target file for writing
    if (!outFile.is_open()) return;          // Abort if the file could not be opened

    outFile << "Role,NationalID,Name,Phone,Email\n";   // Column headers
    for (Admin* a : admins) {                           // Iterate over every admin pointer
        outFile << a->getType()        << ","           // Column 1: role string (always "Admin")
                << a->getNationalID()  << ","           // Column 2: national ID
                << a->getName()        << ","           // Column 3: full name
                << a->getPhoneNumber() << ","           // Column 4: phone number
                << a->getEmail()       << "\n";         // Column 5: email address
    }
    outFile.close();   // Flush and close the file
}

// Writes every super admin account to a CSV file
// Each row contains: Role, NationalID, Name, Phone, Email
void FileManager::saveSuperAdminsToCSV(const vector<SuperAdmin*>& superAdmins, const string& filename) {
    ofstream outFile(filename);                  // Open (or create) the target file for writing
    if (!outFile.is_open()) return;              // Abort if the file could not be opened

    outFile << "Role,NationalID,Name,Phone,Email\n";   // Column headers
    for (SuperAdmin* sa : superAdmins) {               // Iterate over every super admin pointer
        outFile << sa->getType()        << ","         // Column 1: role string (always "SuperAdmin")
                << sa->getNationalID()  << ","         // Column 2: national ID
                << sa->getName()        << ","         // Column 3: full name
                << sa->getPhoneNumber() << ","         // Column 4: phone number
                << sa->getEmail()       << "\n";       // Column 5: email address
    }
    outFile.close();   // Flush and close the file
}

// Writes every individual piece of luggage (from all tickets) to a CSV file
// Each row contains: PassengerID, TicketID, LuggageType, Weight_kg, ExtraChargeApplies
void FileManager::saveLuggageToCSV(const vector<Ticket*>& tickets, const string& filename) {
    ofstream outFile(filename);              // Open (or create) the target file for writing
    if (!outFile.is_open()) return;          // Abort if the file could not be opened

    outFile << "PassengerID,TicketID,LuggageType,Weight_kg,ExtraChargeApplies\n";   // Column headers

    for (Ticket* t : tickets) {                                  // Iterate over every ticket
        vector<Luggage> bags = t->getLuggages();                 // Get all luggage items for this ticket
        for (const Luggage& bag : bags) {                        // Iterate over each piece of luggage
            string bagType    = (bag.getType() == CABIN) ? "Cabin" : "Checked";   // Map enum to string
            string extraCharge = bag.hasExtraCharge() ? "Yes" : "No";             // Indicate if extra fee applies

            outFile << t->getPassengerID() << ","   // Column 1: owner's national ID
                    << t->getTicketID()    << ","   // Column 2: ticket this bag belongs to
                    << bagType             << ","   // Column 3: "Cabin" or "Checked"
                    << bag.getWeight()     << ","   // Column 4: weight in kg
                    << extraCharge         << "\n"; // Column 5: "Yes" or "No" for extra charge
        }
    }
    outFile.close();   // Flush and close the file
}

// Master Save: Triggers all saving functions at once using hard-coded file paths
void FileManager::saveAllSystemData(SystemManager* sys) {
    cout << "\n[FileManager] Saving all data to Excel (CSV) files...\n";
    savePassengersToCSV(sys->getAllPassengers(),  R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Passengers_Data.csv)");   // Save all passengers
    saveFlightsToCSV(sys->getAllFlights(),         R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Flights_Data.csv)");     // Save all flights
    saveTicketsToCSV(sys->getAllTickets(),          R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Tickets_Data.csv)");    // Save all tickets
    saveAdminsToCSV(sys->getAllAdmins(),            R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Admins_Data.csv)");     // Save all admins
    saveSuperAdminsToCSV(sys->getAllSuperAdmins(),  R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\SuperAdmins_Data.csv)"); // Save all super admins
    saveLuggageToCSV(sys->getAllTickets(),          R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Luggage_Data.csv)");   // Save all luggage
    cout << "[FileManager] All data saved successfully!\n";
}

// SECTION 2: REPORTING & ANALYTICS


// Search for a passenger and show their full history (profile + tickets)
void FileManager::generatePassengerReport(SystemManager* sys, const string& searchID) {
    cout << "\n========== PASSENGER REPORT ==========\n";
    Passenger* p = sys->searchPassengerID(searchID);   // Look up the passenger by national ID

    if (p) {
        p->displayInfo();    // Print the passenger's profile fields
        cout << "\n";
        p->viewTickets();    // Print all tickets held by this passenger
    } else {
        cout << "Error: No passenger found with National ID: " << searchID << "\n";   // ID not found
    }
    cout << "======================================\n";
}

// Prints a high-level summary of the entire system: counts of passengers, flights, tickets, and admins
void FileManager::generateSystemSummaryReport(SystemManager* sys) {
    cout << "\n========== GLOBAL SYSTEM SUMMARY ==========\n";
    cout << "Total Passengers Registered : " << sys->getAllPassengers().size() << "\n";   // Passenger count
    cout << "Total Flights Scheduled     : " << sys->getAllFlights().size()    << "\n";   // Flight count
    cout << "Total Tickets Booked        : " << sys->getAllTickets().size()    << "\n";   // Ticket count
    cout << "Total Admins Registered     : " << sys->getAllAdmins().size()     << "\n";   // Admin count
    cout << "===========================================\n";
}

 //SECTION 3: LOADING DATA FROM FILES


void FileManager::loadAllSystemData(SystemManager* sys) {
    cout << "[FileManager] Loading data ...\n";

    // 1. Load SuperAdmins from CSV
    ifstream saFile(R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\SuperAdmins_Data.csv)");
    if (saFile.is_open()) {                     // Only proceed if the file was found and opened
        string line;
        getline(saFile, line);                  // Skip the first line (column headers)
        while (getline(saFile, line)) {         // Read one data row per iteration
            stringstream ss(line);              // Wrap the line in a stringstream for comma-delimited parsing
            string role, id, name, phone, email;
            getline(ss, role,  ',');            // Parse column 1: role (unused, kept for format consistency)
            getline(ss, id,    ',');            // Parse column 2: national ID
            getline(ss, name,  ',');            // Parse column 3: full name
            getline(ss, phone, ',');            // Parse column 4: phone number
            getline(ss, email, ',');            // Parse column 5: email address
            if (!id.empty()) sys->addUser(new SuperAdmin(name, id, phone, email));   // Reconstruct and register
        }
        saFile.close();   // Close the file after reading
    }

    // 2. Load Admins from CSV
    ifstream aFile(R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Admins_Data.csv)");
    if (aFile.is_open()) {                      // Only proceed if the file was found and opened
        string line;
        getline(aFile, line);                   // Skip the header row
        while (getline(aFile, line)) {          // Read one data row per iteration
            stringstream ss(line);              // Wrap for comma-delimited parsing
            string role, id, name, phone, email;
            getline(ss, role,  ',');            // Parse column 1: role (unused)
            getline(ss, id,    ',');            // Parse column 2: national ID
            getline(ss, name,  ',');            // Parse column 3: full name
            getline(ss, phone, ',');            // Parse column 4: phone number
            getline(ss, email, ',');            // Parse column 5: email address
            if (!id.empty()) sys->addUser(new Admin(name, id, phone, email));   // Reconstruct and register
        }
        aFile.close();   // Close the file after reading
    }

    // 3. Load Passengers from CSV
    ifstream pFile(R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Passengers_Data.csv)");
    if (pFile.is_open()) {                       // Only proceed if the file was found and opened
        string line;
        getline(pFile, line);                    // Skip the header row
        while (getline(pFile, line)) {           // Read one data row per iteration
            stringstream ss(line);               // Wrap for comma-delimited parsing
            string id, name, phone, email, pass;
            getline(ss, id,    ',');             // Parse column 1: national ID
            getline(ss, name,  ',');             // Parse column 2: full name
            getline(ss, phone, ',');             // Parse column 3: phone number
            getline(ss, email, ',');             // Parse column 4: email address
            getline(ss, pass,  ',');             // Parse column 5: password
            if (!id.empty()) sys->addUser(new Passenger(name, id, phone, email, pass));   // Reconstruct and register
        }
        pFile.close();   // Close the file after reading
    }

    // 4. Load Flights from CSV
    ifstream fFile(R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Flights_Data.csv)");
    if (fFile.is_open()) {                         // Only proceed if the file was found and opened
        string line;
        getline(fFile, line);                      // Skip the header row
        while (getline(fFile, line)) {             // Read one data row per iteration
            stringstream ss(line);                 // Wrap for comma-delimited parsing
            string idStr, origin, dest, capStr, priceStr, date, time, booked;
            getline(ss, idStr,    ',');            // Parse column 1: flight ID (as string)
            getline(ss, origin,   ',');            // Parse column 2: origin
            getline(ss, dest,     ',');            // Parse column 3: destination
            getline(ss, capStr,   ',');            // Parse column 4: capacity (as string)
            getline(ss, priceStr, ',');            // Parse column 5: price (as string)
            getline(ss, date,     ',');            // Parse column 6: date
            getline(ss, time,     ',');            // Parse column 7: time
            getline(ss, booked,   ',');            // Parse column 8: booked count (read but not used directly)

            if (!idStr.empty()) {
                // Convert string fields to their correct numeric types and construct the Flight
                sys->addFlight(new Flight(stoi(idStr), origin, dest, stoi(capStr), stod(priceStr), date, time));
            }
        }
        fFile.close();   // Close the file after reading
    }

    // 5. Load Tickets and Link them to Passengers/Flights
    ifstream tFile(R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Tickets_Data.csv)");
    if (tFile.is_open()) {                          // Only proceed if the file was found and opened
        string line;
        getline(tFile, line);                       // Skip the header row
        while (getline(tFile, line)) {              // Read one data row per iteration
            stringstream ss(line);                  // Wrap for comma-delimited parsing
            string tID, pID, fID, seat, tClass, weight, price;
            getline(ss, tID,    ',');               // Parse column 1: ticket ID
            getline(ss, pID,    ',');               // Parse column 2: passenger national ID
            getline(ss, fID,    ',');               // Parse column 3: flight ID
            getline(ss, seat,   ',');               // Parse column 4: seat label
            getline(ss, tClass, ',');               // Parse column 5: class string ("Economy"/"Business")
            getline(ss, weight, ',');               // Parse column 6: total luggage weight (informational)
            getline(ss, price,  ',');               // Parse column 7: total price (informational)

            if (!tID.empty()) {
                int ticketID = stoi(tID);                                            // Convert ticket ID to int
                int flightID = stoi(fID);                                            // Convert flight ID to int
                TicketType type = (tClass == "Economy") ? ECONOMY : BUSINESS;       // Map string to enum

                Flight*    f = sys->searchFlightID(flightID);                       // Look up the associated flight
                Passenger* p = sys->searchPassengerID(pID);                         // Look up the associated passenger

                // Re-link the ticket if the dependencies exist
                if (f && p) {
                    Ticket* t = new Ticket(ticketID, pID, flightID, seat, type, f); // Reconstruct the Ticket object
                    sys->addTicket(t);                                               // Register in the global ticket list
                    p->addTicket(t);                                                 // Link to the passenger's personal list
                    f->bookSpecificSeat(seat, type);                                 // Mark the seat as taken on the flight
                }
            }
        }
        tFile.close();   // Close the file after reading
    }

    // 6. Load Luggage and attach it to the correct Tickets
    ifstream lFile(R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Luggage_Data.csv)");
    if (lFile.is_open()) {                           // Only proceed if the file was found and opened
        string line;
        getline(lFile, line);                        // Skip the header row
        while (getline(lFile, line)) {               // Read one data row per iteration
            stringstream ss(line);                   // Wrap for comma-delimited parsing
            string pID, tID, lType, weightStr, extra;
            getline(ss, pID,       ',');             // Parse column 1: passenger ID (unused during load)
            getline(ss, tID,       ',');             // Parse column 2: ticket ID
            getline(ss, lType,     ',');             // Parse column 3: luggage type string
            getline(ss, weightStr, ',');             // Parse column 4: weight in kg (as string)
            getline(ss, extra,     ',');             // Parse column 5: extra charge flag (informational)

            if (!tID.empty()) {
                int         ticketID = stoi(tID);                             // Convert ticket ID to int
                double      weight   = stod(weightStr);                       // Convert weight to double
                LuggageType type     = (lType == "Cabin") ? CABIN : CHECKED; // Map string to enum

                Ticket* t = sys->searchTicketID(ticketID);                    // Look up the parent ticket
                if (t) {
                    t->addLuggage(Luggage(weight, type));                     // Reconstruct and attach the luggage
                }
            }
        }
        lFile.close();   // Close the file after reading
    }
    SystemManager::getInstance()->syncNextTicketID();   // Ensure the next auto-generated ticket ID won't collide
    cout << "[FileManager] All data files successfully restored!\n";
}



// SECTION 4: UTILITY

// Factory Reset: Wipes the data but keeps the column headers for future loads
void FileManager::clearAllFiles() {
    cout << "\n[FileManager] Delete all data from files\n";

    // Rewrite each file with only its header row, effectively deleting all data rows
    ofstream pFile(R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Passengers_Data.csv)");
    if (pFile.is_open()) pFile << "NationalID,Name,Phone,Email,Password\n";       // Passengers header only

    ofstream fFile(R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Flights_Data.csv)");
    if (fFile.is_open()) fFile << "FlightID,Origin,Destination,Capacity,Price,Date,Time,BookedSeats\n";  // Flights header only

    ofstream tFile(R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Tickets_Data.csv)");
    if (tFile.is_open()) tFile << "TicketID,PassengerID,FlightID,SeatNumber,Class,TotalLuggageWeight,TotalPrice\n";  // Tickets header only

    ofstream aFile(R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Admins_Data.csv)");
    if (aFile.is_open()) aFile << "Role,NationalID,Name,Phone,Email\n";            // Admins header only

    ofstream saFile(R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\SuperAdmins_Data.csv)");
    if (saFile.is_open()) saFile << "Role,NationalID,Name,Phone,Email\n";          // SuperAdmins header only

    ofstream lFile(R"(C:\Users\11\Documents\OOP_Project\final_proj\DB\Luggage_Data.csv)");
    if (lFile.is_open()) lFile << "PassengerID,TicketID,LuggageType,Weight_kg,ExtraChargeApplies\n";  // Luggage header only

    cout << "[FileManager] All files have been successfully reset.\n";
}
