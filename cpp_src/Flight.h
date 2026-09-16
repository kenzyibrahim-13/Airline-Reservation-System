#ifndef FLIGHT_H
#define FLIGHT_H

#include <iostream>    // For std::ostream (used in the << operator overload)
#include <string>
#include <vector>
#include <algorithm>   // For std::remove (used to erase a seat from takenSeats)
#include "Type.h"
using namespace std;


class Ticket;   // Forward declaration: Ticket references Flight, so we avoid a circular include



class Flight {
private:
    int    flightID;           // Unique numeric identifier for this flight
    string origin;             // Departure city or airport code
    string destination;        // Arrival city or airport code
    int    capacity;           // Total seat count (max 180: 30 rows x 6 cols)
    double price;              // Base ticket price (before class or luggage surcharges)
    string date;               // Departure date in YYYY-MM-DD format
    string time;               // Departure time in HH:MM format
    vector<string> takenSeats; // List of seat labels that have already been booked (e.g. "3B", "15F")

public:
    // Constructor: initialises all fields; takenSeats starts empty
    Flight(int id, const string& from, const string& to, int cap, double p, const string& d, const string& t);

    // Getters — all const because they do not modify the flight state
    int    getFlightID()     const;   // Returns the numeric flight ID
    string getOrigin()       const;   // Returns the origin city/airport
    string getDestination()  const;   // Returns the destination city/airport
    double getPrice()        const;   // Returns the base ticket price
    string getTime()         const;   // Returns the departure time string
    string getDate()         const;   // Returns the departure date string
    int    getCapacity()     const;   // Returns the total seat capacity
    int    getBookedCount()  const;   // Returns how many seats have been booked so far

    // Returns true when the number of booked seats equals the total capacity
    bool isFull() const;

    // Seat management
    // Books a specific seat if it is valid and available; returns true on success
    bool bookSpecificSeat( string& seat, TicketType type);
    // Returns true if the seat label is NOT already in takenSeats (normalises to uppercase)
    bool isSeatAvailable( string& seat)               const;
    // Returns true if the seat label is syntactically valid and belongs to the correct class rows
    bool isSeatValid( string& seat, TicketType type)  const;
    // Removes a seat label from takenSeats (called when a ticket is cancelled)
    void cancelTakenSeat(const string& seat);

    // Returns a list of all seat labels not yet booked for the given class
    vector<string> getAvailableSeats(TicketType type) const;
    // Prints the available seats for the given class in a grid layout to stdout
    void displayAvailableSeats(TicketType type)        const;

    // Operator overloading: prints full flight details (ID, route, date/time, price, booked count)
    friend ostream& operator<<(ostream& out, const Flight& f);
};

#endif
