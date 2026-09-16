import 'package:flutter/material.dart';
import 'package:final_proj/utils/app_theme.dart';

class Profile extends StatelessWidget {
  final String name;
  final String nationalID;
  final String email;
  final String phoneNumber;
  final int ticketCount;

  const Profile({
    super.key,
    required this.name,
    required this.nationalID,
    required this.email,
    required this.phoneNumber,
    required this.ticketCount,
  });

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: AppTheme.background,
      appBar: AppBar(
        backgroundColor: AppTheme.background,
        elevation: 0,
        leading: IconButton(
          icon: const Icon(Icons.arrow_back_ios, color: AppTheme.mainText),
          onPressed: () => Navigator.pop(context),
        ),
        title: const Text(
          'My Profile',
          style: TextStyle(
            fontSize: 18,
            fontWeight: FontWeight.bold,
            color: AppTheme.mainText,
          ),
        ),
      ),
      body: SafeArea(
        child: SingleChildScrollView(
          padding: const EdgeInsets.all(24),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [

              Center(
                child: Container(
                  padding: const EdgeInsets.all(20),
                  decoration: BoxDecoration(
                    color: AppTheme.passengerAccent.withValues(alpha: 0.1),
                    shape: BoxShape.circle,
                  ),
                  child: const Icon(
                    Icons.person,
                    size: 60,
                    color: AppTheme.passengerAccent,
                  ),
                ),
              ),

              const SizedBox(height: 16),

              Center(
                child: Text(
                  name,
                  style: const TextStyle(
                    fontSize: 24,
                    fontWeight: FontWeight.bold,
                    color: AppTheme.mainText,
                  ),
                ),
              ),

              const SizedBox(height: 32),

              Container(
                decoration: BoxDecoration(
                  color: AppTheme.cardColor,
                  borderRadius: BorderRadius.circular(20),
                  border: Border.all(
                    color: AppTheme.passengerAccent.withValues(alpha: 0.3),
                  ),
                  boxShadow: [
                    BoxShadow(
                      color: Colors.black.withValues(alpha: 0.08),
                      blurRadius: 6,
                      offset: const Offset(2, 4),
                    ),
                  ],
                ),
                child: Column(
                  children: [
                    _infoRow(Icons.badge_outlined, 'National ID', nationalID),
                    Divider(color: AppTheme.passengerAccent.withValues(alpha: 0.2), height: 1),
                    _infoRow(Icons.email_outlined, 'Email', email),
                    Divider(color: AppTheme.passengerAccent.withValues(alpha: 0.2), height: 1),
                    _infoRow(Icons.phone_outlined, 'Phone', phoneNumber),
                    Divider(color: AppTheme.passengerAccent.withValues(alpha: 0.2), height: 1),
                    _infoRow(Icons.confirmation_number_outlined, 'Tickets Booked', ticketCount.toString()),
                  ],
                ),
              ),

            ],
          ),
        ),
      ),
    );
  }

  Widget _infoRow(IconData icon, String label, String value) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 20, vertical: 16),
      child: Row(
        children: [
          Icon(icon, size: 20, color: AppTheme.passengerAccent),
          const SizedBox(width: 16),
          Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(
                label,
                style: const TextStyle(
                  fontSize: 12,
                  color: AppTheme.subtitleText,
                ),
              ),
              Text(
                value,
                style: const TextStyle(
                  fontSize: 15,
                  fontWeight: FontWeight.bold,
                  color: AppTheme.mainText,
                ),
              ),
            ],
          ),
        ],
      ),
    );
  }
}