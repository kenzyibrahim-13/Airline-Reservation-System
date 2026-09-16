#include "Flight.h"
using namespace std;

// Constructor: stores all flight metadata; takenSeats vector initialises to empty by default
Flight::Flight(int id, const string& from, const string& to, int cap, double p, const string& d, const string& t)
    : flightID(id), origin(from), destination(to), capacity(cap), price(p), date(d), time(t) {}

// Simple getters — return the corresponding private member values
int    Flight::getFlightID()    const { return flightID;     }   // Returns the numeric flight ID
string Flight::getOrigin()      const { return origin;       }   // Returns the departure location
string Flight::getDestination() const { return destination;  }   // Returns the arrival location
double Flight::getPrice()       const { return price;        }   // Returns the base ticket price
string Flight::getTime()        const { return time;         }   // Returns the departure time (HH:MM)
string Flight::getDate()        const { return date;         }   // Returns the departure date (YYYY-MM-DD)
int    Flight::getCapacity()    const { return capacity;     }   // Returns the total seat count
int    Flight::getBookedCount() const { return static_cast<int>(takenSeats.size()); }   // Returns how many seats are booked

// Returns true when every seat has been booked (takenSeats count >= capacity)
bool Flight::isFull() const {
    return static_cast<int>(takenSeats.size()) >= capacity;
}

// Checks whether the given seat label is NOT already in takenSeats
// Normalises the seat string to uppercase before comparison to allow case-insensitive input
bool Flight::isSeatAvailable(string& seat) const {
    for (char& c : seat) c = toupper(c);          // Uppercase every character in the seat label
    for (const auto& s : takenSeats)
        if (s == seat) return false;              // Seat found in takenSeats → not available
    return true;                                  // Seat not found → available
}

// Validates that a seat label is syntactically correct and belongs to the right class rows
// Seat format: row number (1-30) followed by column letter (A-F), e.g. "3B", "15F"
bool Flight::isSeatValid( string& seat, TicketType type) const {
    for (char& c : seat) c = toupper(c);          // Normalise to uppercase
    if (seat.size() < 2) return false;            // A valid seat must have at least a digit and a letter

    // Last character is the column letter (A-F)
    char col = seat.back();
    if (col < 'A' || col > 'F') return false;     // Column must be one of A, B, C, D, E, F

    // Everything before the last char is the row number
    int row = stoi(seat.substr(0, seat.size() - 1));   // Parse the numeric row portion

    if (type == BUSINESS)
        return (row >= 1 && row <= 5);            // Business class occupies rows 1–5
    else
        return (row >= 6 && row <= 30);           // Economy class occupies rows 6–30
}

// Attempts to book a specific seat
// Normalises to uppercase, then checks: not full, valid for the class, and not already taken
// Adds the seat to takenSeats and returns true on success; returns false otherwise
bool Flight::bookSpecificSeat( string& seat, TicketType type) {
    // Convert seat to uppercase to handle lowercase input
    for (char& c : seat)
        c = toupper(c);
    if (isFull())                      return false;   // No seats remaining on this flight
    if (!isSeatValid(seat, type))      return false;   // Seat label or class mismatch
    if (!isSeatAvailable(seat))        return false;   // Seat already booked by someone else

    takenSeats.push_back(seat);   // Mark the seat as booked
    return true;                  // Booking successful
}

// Removes a seat label from takenSeats (called when a ticket is cancelled)
// Normalises to uppercase before erasing so the match is case-insensitive
void Flight::cancelTakenSeat(const string& seat) {
    string upperSeat = seat;
    for (char& c : upperSeat) c = toupper(c);   // Normalise to uppercase for consistent comparison
    takenSeats.erase(remove(takenSeats.begin(), takenSeats.end(), seat),   // Erase matching element
                     takenSeats.end()
    );
}

// Returns all available seats for the given class
// Business: rows 1–5; Economy: rows 6–30. Columns always A–F
// Seat format: rowNumber + colLetter, e.g. "1A", "6F", "30C"
vector<string> Flight::getAvailableSeats(TicketType type) const {
    vector<string> available;

    int rowStart = (type == BUSINESS) ? 1  : 6;    // First row for this class
    int rowEnd   = (type == BUSINESS) ? 5  : 30;   // Last row for this class

    for (int row = rowStart; row <= rowEnd; row++) {      // Iterate over each row in the class
        for (char col = 'A'; col <= 'F'; col++) {         // Iterate over each column (A–F)
            string seat = to_string(row) + col;           // Build the seat label (e.g. "3B")
            if (isSeatAvailable(seat))
                available.push_back(seat);                // Include only if not already booked
        }
    }
    return available;   // Return the list of all unbooked seats in the requested class
}

// Prints all available seats for the given class to stdout in a grid layout (6 seats per line)
void Flight::displayAvailableSeats(TicketType type) const {
    vector<string> seats = getAvailableSeats(type);               // Get the available seat list
    string className = (type == BUSINESS) ? "Business (rows 1-5)"
                                          : "Economy  (rows 6-30)";   // Human-readable class label
    cout << "Available " << className << " seats on Flight " << flightID << ":\n";
    if (seats.empty()) {
        cout << "  No seats available.\n";   // All seats in this class are booked
        return;
    }
    for (int i = 0; i < static_cast<int>(seats.size()); i++) {
        cout << seats[i];                                            // Print the seat label
        if (i + 1 < static_cast<int>(seats.size())) cout << "  ";   // Separate seats with two spaces
        if ((i + 1) % 6 == 0) cout << "\n";                         // New line after every 6 seats for readability
    }
    cout << "\n";
}

// Overloaded stream insertion operator: prints a formatted summary of the flight
ostream& operator<<(ostream& out, const Flight& f) {
    out << "Flight ID : " << f.flightID                          << "\n"   // Numeric flight ID
        << "Route     : " << f.origin << " -> " << f.destination << "\n"   // Origin → Destination
        << "Date/Time : " << f.date   << " at " << f.time        << "\n"   // Full departure date and time
        << "Price     : $" << f.price                            << "\n"   // Base ticket price
        << "Seats     : " << f.takenSeats.size()
                          << " / " << f.capacity << " booked"    << "\n";  // Booked vs total capacity
    return out;   // Return the stream to allow chaining
}
