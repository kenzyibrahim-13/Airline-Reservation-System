import 'package:final_proj/Services/ffi_bridge.dart';
import 'package:flutter/material.dart';
import 'package:final_proj/Screens/profile.dart';
import 'package:final_proj/Widgets/flight_card.dart';
import 'package:final_proj/Widgets/ticket_card.dart';
import 'package:final_proj/utils/app_theme.dart';

class PassengerHome extends StatefulWidget {
  final String name;
  final String nationalID;
  final String email;
  final String phoneNumber;
  final List<Map<String, dynamic>> flights;
  final List<Map<String, dynamic>> tickets;

  const PassengerHome({
    super.key,
    required this.name,
    required this.nationalID,
    required this.email,
    required this.phoneNumber,
    required this.flights,
    required this.tickets,
  });

  @override
  State<PassengerHome> createState() => _PassengerHomeState();
}

class _PassengerHomeState extends State<PassengerHome> {
  late List<Map<String, dynamic>> currentTickets;
  late List<Map<String, dynamic>> currentFlights;

  @override
  void initState() {
    currentTickets = List.from(widget.tickets);
    currentFlights = List.from(widget.flights);
    super.initState();
  }

  void _refreshTickets() {
    setState(() {
      currentTickets = AirlineBridge.getTicketsForPassenger(widget.nationalID);
    });
  }

  void _refreshFlights() {
    setState(() {
      currentFlights = AirlineBridge.getAllFlights();
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: AppTheme.background,
      appBar: AppBar(
        backgroundColor: AppTheme.background,
        elevation: 0,
        title: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text(
              'Welcome,',
              style: TextStyle(fontSize: 14, color: AppTheme.subtitleText),
            ),
            Text(
              widget.name,
              style: const TextStyle(
                fontSize: 18,
                fontWeight: FontWeight.bold,
                color: AppTheme.mainText,
              ),
            ),
          ],
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
                    ticketCount: currentTickets.length,
                  ),
                ),
              );
              _refreshTickets();
            },
          ),
          const SizedBox(width:20)
        ],
      ),
      body: SafeArea(
        child: Column(
          children: [

            // ── Top half: Available Flights ──────────────────────────────
            Expanded(
              child: Padding(
                padding: const EdgeInsets.fromLTRB(24, 16, 24, 8),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const Text(
                      'Available Flights',
                      style: TextStyle(
                        fontSize: 20,
                        fontWeight: FontWeight.bold,
                        color: AppTheme.mainText,
                      ),
                    ),
                    const SizedBox(height: 12),
                    Expanded(
                      child: currentFlights.isEmpty
                          ? _emptyState(
                              icon: Icons.flight_outlined,
                              message: 'No flights available at the moment.',
                            )
                          : ListView.builder(
                              itemCount: currentFlights.length,
                              itemBuilder: (context, index) {
                                final flight = currentFlights[index];
                                return FlightCard(
                                  passengerID: widget.nationalID,
                                  flightID: int.parse(flight['flightID'].toString()),
                                  origin: flight['origin'],
                                  destination: flight['destination'],
                                  date: flight['date'],
                                  time: flight['time'],
                                  price: flight['price'],
                                  isFull: flight['isFull'],
                                  themeColor: AppTheme.passengerAccent,
                                  onBooked: () {
                                    _refreshTickets();
                                    _refreshFlights();
                                  },
                                );
                              },
                            ),
                    ),
                  ],
                ),
              ),
            ),

            // ── Divider ──────────────────────────────────────────────────
            Divider(
              height: 1,
              thickness: 1,
              color: Colors.grey.shade200,
            ),

            // ── Bottom half: My Tickets ──────────────────────────────────
            Expanded(
              child: Padding(
                padding: const EdgeInsets.fromLTRB(24, 16, 24, 8),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const Text(
                      'My Tickets',
                      style: TextStyle(
                        fontSize: 20,
                        fontWeight: FontWeight.bold,
                        color: AppTheme.mainText,
                      ),
                    ),
                    const SizedBox(height: 12),
                    Expanded(
                      child: currentTickets.isEmpty
                          ? _emptyState(
                              icon: Icons.confirmation_number_outlined,
                              message: 'You have no booked tickets yet.',
                            )
                          : ListView.builder(
                              itemCount: currentTickets.length,
                              itemBuilder: (context, index) {
                                final ticket = currentTickets[index];
                                return TicketCard(
                                  ticketID: ticket['ticketID'],
                                  flightID: ticket['flightID'],
                                  seatNumber: ticket['seatNumber'],
                                  isBusiness: ticket['isBusiness'],
                                  totalWeight: ticket['totalWeight'],
                                  hasExtraFees: ticket['hasExtraFees'],
                                  extraFee: ticket['extraFee'],
                                  totalPrice: ticket['totalPrice'],
                                  nationalID: widget.nationalID,
                                  themeColor: AppTheme.passengerAccent,
                                  onCancel: () {
                                    _refreshFlights();
                                    _refreshTickets();
                                  },
                                );
                              },
                            ),
                    ),
                  ],
                ),
              ),
            ),

          ],
        ),
      ),
    );
  }

  Widget _emptyState({required IconData icon, required String message}) {
    return Center(
      child: Padding(
        padding: const EdgeInsets.symmetric(vertical: 32),
        child: Column(
          children: [
            Icon(icon, size: 48, color: AppTheme.subtitleText.withValues(alpha: 0.5)),
            const SizedBox(height: 12),
            Text(
              message,
              style: const TextStyle(fontSize: 14, color: AppTheme.subtitleText),
            ),
          ],
        ),
      ),
    );
  }
}