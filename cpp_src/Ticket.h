#ifndef TICKET_H
#define TICKET_H

#include <iostream>    // For std::ostream (used in the << operator overload)
#include <string>
#include <vector>
#include "Luggage.h"
#include "Type.h"
using namespace std;

class Flight;   // Forward declaration: Ticket holds a Flight* but including Flight.h would be circular

// Represents a booked ticket linking a Passenger to a Flight seat
// Luggage items are attached to the ticket
// Price = flight base price + business surcharge (if applicable) + extra luggage fees
class Ticket {
private:
    int        ticketID;      // Unique numeric identifier for this ticket
    string     passengerID;   // National ID of the passenger who owns this ticket
    int        flightID;      // ID of the flight this ticket is booked on
    string     seatNumber;    // Seat label (e.g. "3B"); already validated by Flight::bookSpecificSeat
    TicketType type;          // ECONOMY or BUSINESS; affects weight limit and price
    vector<Luggage> luggages; // All luggage items attached to this ticket
    Flight* flight;           // Non-owning pointer to the Flight (used to retrieve the base price)

public:
    // Constructor: initialises all fields; the Flight pointer must be valid (non-null)
    Ticket(int tID, const string& pID, int fID, const string& seat, TicketType t, Flight* f);

    // Getters — all const because they do not modify ticket state
    int        getTicketID()    const;   // Returns the unique ticket ID
    string     getPassengerID() const;   // Returns the owning passenger's national ID
    int        getFlightID()    const;   // Returns the associated flight's ID
    string     getSeatNumber()  const;   // Returns the seat label (e.g. "15C")
    TicketType getType()        const;   // Returns ECONOMY or BUSINESS
    vector<Luggage> getLuggages() const; // Returns a copy of all luggage on this ticket

    // Luggage management
    void   addLuggage(const Luggage& l);   // Appends a luggage item to this ticket
    double getTotalWeight()    const;      // Returns the sum of all luggage weights in kg
    double getWeightLimit()    const;      // 20 kg for Economy, 30 kg for Business
    bool   hasExtraFees()      const;      // True if total weight exceeds the class allowance
    double calculateExtraFee() const;      // $50 per extra kg over the allowance

    // Total price: base + business upgrade + luggage fees
    double calculateTotalPrice() const;

    // Operator overloading: print full ticket details (seat, class, luggage, price)
    friend ostream& operator<<(ostream& out, const Ticket& t);
};

#endif
