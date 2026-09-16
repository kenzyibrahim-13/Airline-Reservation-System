# Airline-Reservation-System

Let's Travel – Airline Reservation System is a robust, modular desktop application developed in modern C++ with an interactive Flutter GUI. Designed around core Object-Oriented Programming (OOP) and Software Architecture patterns, the platform manages comprehensive flight operations, user authentication, multi-tiered access, and persistent flight data.
Key Features
Role-Based Access Control: Three distinct authorization tiers—Passenger (search, book, luggage selection, cancellations), Admin (schedule flights, manage passenger records), and SuperAdmin (admin management, full system reset).
Interactive Frontend: A cross-platform UI built with Flutter connected directly to the C++ core via Dart FFI (Foreign Function Interface) and native C-bridge bindings. 
Booking & Dynamic Pricing Engine: Handles seat selection across Business and Economy classes, cabin/checked luggage weight constraints, and automatic overweight fee calculations.
Data Persistence: Integrated FileManager repository layer that reads, writes, and synchronizes system states, flights, and tickets with CSV files across restarts. 
Robust Software Architecture: Utilizes industry design patterns including Singleton (SystemManager), Factory Method (User creation), Adapter (C++ to Dart FFI bridge), and Template Method.  
