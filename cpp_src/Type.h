#ifndef TYPE_H
#define TYPE_H

// Enum for ticket class: determines seat rows, weight allowance, and price surcharge
enum TicketType {
    ECONOMY,
    BUSINESS
};

// Enum for luggage category: determines whether extra-weight rules apply
enum LuggageType {
    CABIN,      // Carry-on luggage; no extra-charge rules apply
    CHECKED     // Hold luggage; extra fee if weight > 23 kg
};

// Enum for user role: used by the User factory method to decide which derived class to create
enum UserType {
    PASSENGER,  // Regular customer who can book tickets
    ADMIN       // Staff member who manages flights and users
};

#endif
