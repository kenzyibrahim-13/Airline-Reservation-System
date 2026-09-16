#include "Luggage.h"
using namespace std;

// Constructor: stores the supplied weight and luggage type
Luggage::Luggage(double w, LuggageType t) : weight(w), type(t) {}

// Returns the weight of luggage in kilograms
double Luggage::getWeight() const {
    return weight;
}

// Returns the luggage type (CABIN for carry-on, CHECKED for hold luggage)
LuggageType Luggage::getType() const {
    return type;
}

// Returns true if an extra baggage fee applies
// Extra charge applies only to checked luggage heavier than 23 kg
bool Luggage::hasExtraCharge() const {
    return (type == CHECKED && weight > 23.0);   // Only CHECKED bags over 23 kg incur extra fees
}

// Prints a single-line summary of this luggage item to stdout
// Includes the type, weight, and a notice if an extra charge applies
void Luggage::print() const {
    cout << "Type: " << (type == CABIN ? "Cabin" : "Checked")   // Display "Cabin" or "Checked"
         << " | Weight: " << weight << " kg";                    // Display weight in kg
    if (hasExtraCharge())
        cout << " [Extra charge applies]";                        // Append notice if overweight checked luggage
    cout << endl;                                                 // End the line and flush the buffer
}
