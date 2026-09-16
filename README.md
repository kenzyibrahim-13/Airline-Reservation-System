# Let's Travel – Airline Reservation System

A modular desktop application developed in **C++** featuring an interactive **Flutter GUI**. The system applies Object-Oriented Programming (OOP) principles and design patterns to manage flight operations, user authentication, seat reservations, and persistent storage.

---

## Features

- **Role-Based Access Control:**
  - **Passenger:** Search and view flights, book seats (Business/Economy), manage luggage, and cancel tickets.
  - **Admin:** Add/cancel flights, manage passenger records, and view system overviews.
  - **SuperAdmin:** Manage administrator accounts and perform factory system resets.
- **Frontend & Bridge:** Built with **Flutter** and connected to native C++ logic via **Dart FFI** and a dedicated C-bridge layer (`bridge.cpp` / `bridge.h`).
- **Luggage & Pricing Logic:** Real-time calculation of total ticket fares based on class upgrades and excess baggage weight fees.
- **Data Persistence:** Integrated `FileManager` reads and writes all records (flights, users, tickets, luggage) using CSV files.
- **System Architecture & Patterns:**
  - **Singleton Pattern:** `SystemManager` provides a centralized in-memory data store.
  - **Factory Pattern:** `User::build()` handles validated role-based object instantiation.
  - **Adapter Pattern:** Exposes clean C interfaces for Dart FFI communication.

---

## Contributors (Egypt University of Informatics)

- **Ahmed Mohamed** – GUI & Bridge Implementation
- **Jana Ahmed** – Admin Class, SystemManager & UML Architecture
- **Ahmed Hesham** – File Manager & Authentication System
- **Lila Mostafa** – User Class, SystemManager Class & UML Architecture
- **Kenzy Elborollosy** – Flight, Luggage, Ticket, SuperAdmin, UserFactory & Testing
