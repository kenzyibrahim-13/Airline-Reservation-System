cmake_minimum_required(VERSION 3.10)
project(airline_bridge)

set(CMAKE_CXX_STANDARD 17)

add_library(airline_bridge SHARED
    Admin.cpp
    AuthSystem.cpp
    bridge.cpp
    FileManager.cpp
    Flight.cpp
    Luggage.cpp
    Passenger.cpp
    SuperAdmin.cpp
    SystemManager.cpp
    Ticket.cpp
    User.cpp
)

target_include_directories(airline_bridge PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(airline_bridge PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../windows/runner"
)