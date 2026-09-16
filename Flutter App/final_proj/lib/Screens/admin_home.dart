import 'dart:ui';

import 'package:final_proj/Services/ffi_bridge.dart';
import 'package:final_proj/Widgets/add_admin_dialog.dart';
import 'package:final_proj/Widgets/cancel_flight.dart';
import 'package:final_proj/Widgets/cancel_ticket.dart';
import 'package:final_proj/Widgets/check_seats_dialog.dart';
import 'package:final_proj/Screens/profile.dart';
import 'package:final_proj/Widgets/flight_card.dart';
import 'package:final_proj/Widgets/remove_admin_dialog.dart';  
import 'package:flutter/material.dart';
import 'package:final_proj/Widgets/admin_action_card.dart';
import 'package:final_proj/Widgets/add_flight_dialog.dart';
import 'package:final_proj/Widgets/edit_user_dialog.dart';
import 'package:final_proj/utils/app_theme.dart';

class AdminHome extends StatefulWidget{
  final String name;
  final String nationalID;
  final String email;
  final String phoneNumber;
  final List<Map<String, dynamic>> allFlights;
  final List<Map<String, dynamic>> allUsers;
  final bool isSup;

  const AdminHome({
    super.key,
    required this.name,
    required this.nationalID,
    required this.email,
    required this.phoneNumber,
    required this.allFlights,
    required this.allUsers,
    this.isSup = false
  });

  @override
  State<AdminHome> createState() => _AdminHomeState();

}

class _AdminHomeState extends State<AdminHome> {
  late List<Map<String, dynamic>> currentFlights;
  late List<Map<String, dynamic>> currentUsers;

  @override
  void initState(){
    super.initState();
    currentFlights = List.from(widget.allFlights);
    currentUsers = List.from(widget.allUsers);
  }

  void _refresh(){
    setState(() {
      currentFlights = AirlineBridge.getAllFlights();
      currentUsers = AirlineBridge.getAllUsers();
    });
  }

    Widget _emptyState(IconData icon, String message) {
    return Center(
      child: Padding(
        padding: const EdgeInsets.symmetric(vertical: 20),
        child: Column(
          children: [
            Icon(icon, size: 40, color: AppTheme.subtitleText.withValues(alpha: 0.3)),
            const SizedBox(height: 8),
            Text(message, style: const TextStyle(color: AppTheme.subtitleText)),
          ],
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: AppTheme.background,
      appBar: AppBar(
        backgroundColor: AppTheme.background,
        elevation: 0,
        title: Text(
          widget.isSup ? "Super Admin: ${widget.name}" : "Admin: ${widget.name}", 
          style: const TextStyle(color: AppTheme.mainText, fontWeight: FontWeight.bold)
        ),
        actions: [
          IconButton(
            icon: const Icon(Icons.person_outline, color: AppTheme.mainText),
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(
                  builder: (context) => Profile(
                    name: widget.name,
                    nationalID: widget.nationalID,
                    email: widget.email,
                    phoneNumber: widget.phoneNumber,
                    ticketCount: 0, // Admins don't have personal tickets
                  ),
                ),
              );
            },
          )
        ],
      ),
      body: SingleChildScrollView(
      padding: const EdgeInsets.all(24),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          const Text("Quick Actions", style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold, color: AppTheme.mainText)),
          const SizedBox(height: 16),
          ScrollConfiguration(
            behavior: ScrollConfiguration.of(context).copyWith(
              dragDevices: {
                PointerDeviceKind.touch,
                PointerDeviceKind.mouse,
                PointerDeviceKind.trackpad
              },
            ),
            child: SingleChildScrollView(
              scrollDirection: Axis.horizontal,
              child: Row(children: [
                AdminActionCard(icon: Icons.flight_takeoff, label: 'Add Flight',
                  onTap: () => showDialog(context: context,
                    builder: (_) => AddFlightDialog(onFlightAdded: _refresh))),
                AdminActionCard(icon: Icons.cancel, label: 'Cancel Flight',
                  onTap: () => showDialog(context: context,
                    builder: (_) => CancelFlightDialog(onFlightCancelled: _refresh))),
                AdminActionCard(icon: Icons.confirmation_number_outlined, label: 'Cancel Ticket',
                  onTap: () => showDialog(context: context,
                    builder: (_) => const CancelTicketDialog())),
                AdminActionCard(icon: Icons.edit_note, label: 'Edit Passenger',
                  onTap: () => showDialog(context: context,
                    builder: (_) => const EditUserDialog())),
                AdminActionCard(icon: Icons.event_seat, label: 'Check Seats',
                  onTap: () => showDialog(context: context,
                    builder: (_) => const CheckSeatsDialog())),
                if (widget.isSup) ...[
                    AdminActionCard(
                      icon: Icons.admin_panel_settings,
                      label: 'Add Admin',
                      onTap: () => showDialog(
                        context: context,
                        builder: (_) => AddAdminDialog(onAdminAdded: _refresh),
                      ),
                    ),
                    AdminActionCard(
                      icon: Icons.remove_moderator,
                      label: 'Remove Admin',
                      onTap: () => showDialog(
                        context: context,
                        builder: (_) => RemoveAdminDialog(onAdminRemoved: _refresh),
                      ),
                    ),
                  ],
              ]),
            ),
          ),
          const SizedBox(height: 32),
          const Text("All Flights", style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold, color: AppTheme.mainText)),
          const SizedBox(height: 12),
          currentFlights.isEmpty
            ? _emptyState(Icons.flight_outlined, "No flights in the system.")
            : Column(children: currentFlights.map((flight) => FlightCard(
                isAdmin: true,              // ← view only
                passengerID: '',
                flightID: int.parse(flight['flightID'].toString()),
                origin: flight['origin'],
                destination: flight['destination'],
                date: flight['date'],
                time: flight['time'],
                price: flight['price'],
                isFull: flight['isFull'],
                themeColor: AppTheme.adminAccent,
                onBooked: () {},
              )).toList()),

          const SizedBox(height: 32),
          const Text("Registered Users", style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold, color: AppTheme.mainText)),
          const SizedBox(height: 12),
          currentUsers.isEmpty
            ? _emptyState(Icons.people_outline, "No passengers registered yet.")
            : Column(children: currentUsers.map((user) => Card(
                color: AppTheme.cardColor,
                margin: const EdgeInsets.only(bottom: 12),
                shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12),
                  side: BorderSide(color: Colors.grey.shade200)),
                child: ListTile(
                  leading: CircleAvatar(
                    backgroundColor: AppTheme.adminAccent.withValues(alpha: 0.1),
                    child: const Icon(Icons.person, color: AppTheme.adminAccent)),
                  title: Text(user['name'], style: const TextStyle(fontWeight: FontWeight.bold, color: AppTheme.mainText)),
                  subtitle: Text("ID: ${user['nationalID']}", style: const TextStyle(color: AppTheme.subtitleText)),
                ),
              )).toList()),
          ],
        ),
     ),
    );
  }
}