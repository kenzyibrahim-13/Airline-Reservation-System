import 'package:final_proj/Screens/admin_home.dart';
import 'package:final_proj/Services/ffi_bridge.dart';
import 'package:flutter/material.dart';
import 'package:final_proj/Screens/login.dart';
import 'package:final_proj/Screens/passenger_home.dart';
import 'package:final_proj/utils/app_theme.dart';
import 'package:final_proj/widgets/role_selection_button.dart';

class RoleSelectionScreen extends StatelessWidget {
  const RoleSelectionScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: AppTheme.background,
      body: SafeArea(
        child: Center(
          child: SingleChildScrollView(
            padding: const EdgeInsets.all(24.0),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Container(
                  padding: const EdgeInsets.all(20),
                  decoration: BoxDecoration(
                    color: AppTheme.passengerAccent.withValues(alpha: 0.1),
                    shape: BoxShape.circle,
                  ),
                  child: const Icon(
                    Icons.flight_takeoff,
                    size: 80,
                    color: AppTheme.passengerAccent,
                  ),
                ),
                const SizedBox(height: 40),
                
                // Welcome Text
                const Text(
                  'Airline Booking System',
                  style: TextStyle(
                    fontSize: 28,
                    fontWeight: FontWeight.bold,
                    color: AppTheme.mainText,
                  ),
                ),
                const Text(
                  'Please select your role to continue',
                  style: TextStyle(
                    fontSize: 16,
                    color: AppTheme.subtitleText,
                  ),
                ),
                const SizedBox(height: 50),

                RoleSelectionButton(
                  title: 'I am a Passenger',
                  subtitle: 'Search flights and book tickets',
                  icon: Icons.person,
                  themeColor: AppTheme.passengerAccent,
                  onTap: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) => Login(
                        title: "Passenger Login",
                        icon: Icons.person,
                        themeColor: AppTheme.passengerAccent,
                        onLogin: (id, password) {
                            final success = AirlineBridge.passengerLogin(id, password);
                            if (!success) {
                              ScaffoldMessenger.of(context).showSnackBar(
                                const SnackBar(
                                  content: Text('Invalid ID or password'),
                                  backgroundColor: AppTheme.subtitleText,
                                ),
                              );
                              return;
                            }
                            final info = AirlineBridge.getPassengerInfo(id);
                            final flights = AirlineBridge.getAllFlights();
                            final tickets = AirlineBridge.getTicketsForPassenger(id);
                            Navigator.push(
                              context,
                              MaterialPageRoute(
                                builder: (context) => PassengerHome(
                                  name: info['name']!,
                                  nationalID: id,
                                  email: info['email']!,
                                  phoneNumber: info['phone']!,
                                  flights: flights,
                                  tickets: tickets,
                                ),
                              ),
                            );
                          },
                        isPassengerLogin: true,
                        )
                      )
                    );
                  },
                ),
                
                const SizedBox(height: 20),

                RoleSelectionButton(
                  title: 'I am an Admin',
                  subtitle: 'Manage flights, tickets, and users',
                  icon: Icons.admin_panel_settings,
                  themeColor: AppTheme.adminAccent,
                  onTap: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context) => Login(
                          title: "Admin Login",
                          icon: Icons.admin_panel_settings,
                          themeColor: AppTheme.adminAccent,
                          isPassengerLogin: false,
                          onLogin: (id, password) {
                            final isSup = AirlineBridge.superAdminLogin(id, password);
                            bool success = isSup;
                            if(!isSup){
                              success = AirlineBridge.adminLogin(id, password);
                            }
                            if (!success) {
                              ScaffoldMessenger.of(context).showSnackBar(
                                const SnackBar(
                                  content: Text('Invalid admin credentials'),
                                  backgroundColor: AppTheme.subtitleText,
                                ),
                              );
                              return;
                            }
                            final flights = AirlineBridge.getAllFlights();
                            final users   = AirlineBridge.getAllUsers();
                            Navigator.push(
                              context,
                              MaterialPageRoute(
                                builder: (context) => AdminHome(
                                  name: id,
                                  nationalID: id,
                                  email: '',
                                  phoneNumber: '',
                                  allFlights: flights,
                                  allUsers: users,
                                  isSup: isSup,
                                ),
                              ),
                            );
                          },
                        )
                      )  
                    );
                  },
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}