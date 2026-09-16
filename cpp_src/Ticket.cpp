#include "Ticket.h"
#include "Flight.h"    // Full Flight definition needed to call flight->getPrice() in calculateTotalPrice
using namespace std;

// Constructor: stores all ticket fields
// The Flight pointer must remain valid for the lifetime of this ticket (non-owning borrow)
Ticket::Ticket(int tID, const string& pID, int fID, const string& seat, TicketType t, Flight* f)
    : ticketID(tID), passengerID(pID), flightID(fID), seatNumber(seat), type(t), flight(f) {}

// Simple getters — return the corresponding private member values
int        Ticket::getTicketID()    const { return ticketID;    }   // Unique ticket ID
string     Ticket::getPassengerID() const { return passengerID; }   // Owner's national ID
int        Ticket::getFlightID()    const { return flightID;    }   // Associated flight ID
string     Ticket::getSeatNumber()  const { return seatNumber;  }   // Seat label (e.g. "3B")
TicketType Ticket::getType()        const { return type;        }   // ECONOMY or BUSINESS

// Returns a copy of the luggage vector (caller gets a snapshot, not a reference)
vector<Luggage> Ticket::getLuggages() const {
    return luggages;
}

// Appends a new luggage item to this ticket's luggage list
void Ticket::addLuggage(const Luggage& l) {
    luggages.push_back(l);   // Store a copy of the Luggage value object
}

// Returns the sum of all luggage weights in kilograms
double Ticket::getTotalWeight() const {
    double total = 0;
    for (const auto& l : luggages)
        total += l.getWeight();   // Accumulate each piece's weight
    return total;
}

// Economy allowance: 20 kg | Business allowance: 30 kg
// Returns the free weight allowance for this ticket's class
double Ticket::getWeightLimit() const {
    return (type == ECONOMY) ? 20.0 : 30.0;   // Economy: 20 kg, Business: 30 kg
}

// Returns true if the total luggage weight exceeds the class allowance.
bool Ticket::hasExtraFees() const {
    return getTotalWeight() > getWeightLimit();   // Any overage triggers extra fees
}

// $50 charged for every kg over the allowance.
// Returns 0.0 if the total weight is within the free allowance.
double Ticket::calculateExtraFee() const {
    double extra = getTotalWeight() - getWeightLimit();   // Overage in kg (may be negative)
    return (extra > 0) ? extra * 50.0 : 0.0;             // Only charge for positive overage
}

// Calculates the full price of this ticket
// Base price comes from the flight object
// Business class adds a fixed $1000 surcharge on top of the base price
// Any excess luggage fee is added last
double Ticket::calculateTotalPrice() const {
    double total = flight->getPrice();            // Start with the flight's base ticket price
    if (type == BUSINESS) total += 1000.0;        // Add fixed business class surcharge
    total += calculateExtraFee();                 // Add any excess luggage fees
    return total;
}

// Overloaded stream insertion operator: prints a formatted summary of the full ticket
ostream& operator<<(ostream& out, const Ticket& t) {
    out << "-------------------------------\n"
        << "Ticket ID   : " << t.ticketID                              << "\n"   // Unique ticket ID
        << "Passenger ID: " << t.passengerID                           << "\n"   // Owner's national ID
        << "Flight ID   : " << t.flightID                              << "\n"   // Associated flight
        << "Seat        : " << t.seatNumber                            << "\n"   // Seat label
        << "Class       : " << (t.type == ECONOMY ? "Economy" : "Business") << "\n";  // Class name

    out << "Luggage     : ";
    if (t.luggages.empty()) {
        out << "None\n";   // No luggage attached to this ticket
    } else {
        out << "\n";
        for (const auto& l : t.luggages) {
            out << "  - " << (l.getType() == CABIN ? "Cabin" : "Checked")   // Luggage type label
                << " | " << l.getWeight() << " kg";                          // Weight in kg
            if (l.hasExtraCharge()) out << " [+extra charge]";               // Flag if overweight checked bag
            out << "\n";
        }
        out << "Total Weight: " << t.getTotalWeight() << " kg"          // Sum of all bag weights
            << " (limit: " << t.getWeightLimit() << " kg)\n";           // Free allowance for comparison
    }

    if (t.hasExtraFees())
        out << "Extra Luggage Fee: $" << t.calculateExtraFee() << "\n"; // Show fee only when applicable

    out << "Total Price : $" << t.calculateTotalPrice() << "\n"         // Final ticket price
        << "-------------------------------\n";
    return out;   // Return stream to allow chaining
}
