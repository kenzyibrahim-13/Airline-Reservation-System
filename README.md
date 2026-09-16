# ✈️ Airline Reservation System

A full-stack airline reservation system built as a class project, combining a **C++ backend** for core booking logic with a **Flutter application** for the user-facing experience.

## 📋 Overview

This project simulates a real-world airline reservation platform, allowing users to search for flights, book tickets, and manage reservations, while an administrator can manage flights, schedules, and system data.

## 🛠️ Tech Stack

- **Backend / Core Logic:** C++ (`cpp_src`) — handles the booking engine, business logic, and data management
- **Frontend:** Flutter (`Flutter App/final_proj`) — cross-platform mobile application for both customers and admins
- **Build System:** CMake

## ✨ Features

- Flight search and browsing
- Ticket booking and reservation management
- Customer-facing booking flow
- Admin dashboard for managing flights and reservations
- Cross-platform mobile support (Android/iOS via Flutter)

## 📁 Project Structure

```
Airline-Reservation-System/
├── cpp_src/              # C++ backend — booking engine & core logic
├── Flutter App/
│   └── final_proj/       # Flutter mobile application
├── Presentation.pptx     # Project presentation slides
└── README.md
```

## 🚀 Getting Started

### Prerequisites

- [CMake](https://cmake.org/) (for building the C++ backend)
- A C++ compiler (e.g., GCC, Clang, or MSVC)
- [Flutter SDK](https://flutter.dev/) (for running the mobile app)

### Building the C++ Backend

```bash
cd cpp_src
mkdir build && cd build
cmake ..
cmake --build .
```

### Running the Flutter App

```bash
cd "Flutter App/final_proj"
flutter pub get
flutter run
```

- ## 👥 Contributors

- Kenzy Ibrahim — [@kenzyibrahim-13](https://github.com/kenzyibrahim-13)
- Lila Mostafa — [@LilaSadik](https://github.com/LilaSadik)
- Ahmed Mohamed — [@VastDesert](https://github.com/VastDesert)

## 📄 License

This project was developed for academic purposes as part of a university course.
