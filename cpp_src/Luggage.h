#ifndef LUGGAGE_H
#define LUGGAGE_H

#include <iostream>
#include "Type.h"
using namespace std;

// Represents a single piece of luggage attached to a Ticket
// A ticket can hold multiple Luggage objects
// Extra fees apply only to checked luggage that weighs more than 23 kg
class Luggage {
private:
    double      weight;   // Weight of this piece of luggage in kilograms
    LuggageType type;     // Whether this is CABIN (carry-on) or CHECKED (hold) luggage

public:
    // Constructor: initialises weight and type
    Luggage(double w, LuggageType t);

    double      getWeight()       const;   // Returns the weight in kg
    LuggageType getType()         const;   // Returns the luggage type (CABIN or CHECKED)
    bool        hasExtraCharge()  const;   // Returns true if this is CHECKED luggage heavier than 23 kg

    // Prints a single-line summary of this piece of luggage to stdout,
    // including whether an extra charge applies.
    void print() const;
};

#endif
