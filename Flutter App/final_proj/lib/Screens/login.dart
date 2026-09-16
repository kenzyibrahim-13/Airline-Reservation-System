import 'package:final_proj/Services/ffi_bridge.dart';
import 'package:flutter/material.dart';
import 'package:final_proj/Screens/signup.dart';
import 'package:final_proj/Widgets/custem_text_field.dart';
import 'package:final_proj/utils/app_theme.dart';

class Login extends StatefulWidget {
  final String title;
  final IconData icon;
  final Color themeColor;
  final void Function(String email, String password) onLogin;
  final bool isPassengerLogin;

  const Login({
    super.key,
    required this.title,
    required this.icon,
    required this.themeColor,
    required this.onLogin,
    this.isPassengerLogin = false
  });

  @override
  State<Login> createState() => _LoginState();
}

class _LoginState extends State<Login> {
  final TextEditingController _nationalIDController = TextEditingController();
  final TextEditingController _passwordController = TextEditingController();

  @override
  void dispose() {
    _nationalIDController.dispose();
    _passwordController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: AppTheme.background,
      appBar: AppBar(
        backgroundColor: AppTheme.background,
        elevation: 1,
        leading: IconButton(
          icon: const Icon(Icons.arrow_back_ios, color: AppTheme.mainText),
          onPressed: () => Navigator.pop(context),
        ),
      ),
      body: SafeArea(
        child: Center(
          child: Padding(
            padding: const EdgeInsets.all(24.0),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.center,
              children: [
                Center(
                  child : Container(
                    padding: const EdgeInsets.all(20),
                    decoration: BoxDecoration(
                      color: widget.themeColor.withValues(alpha: 0.1),
                      shape: BoxShape.circle,
                    ),
                    child: Icon(widget.icon, size: 60, color: widget.themeColor),
                  ),
                ),
                const SizedBox(height: 30),
                Center(
                  child : Text(
                    widget.title,
                    style: const TextStyle(
                      fontSize: 28,
                      fontWeight: FontWeight.bold,
                      color: AppTheme.mainText,
                    ),
                  )
                ),
                const SizedBox(height: 8),
                Center(
                  child : const Text(
                    'Enter your credentials to continue',
                    style: TextStyle(fontSize: 16, color: AppTheme.subtitleText),
                  ),
                ),
                const SizedBox(height: 40),
                const Text(
                    'National ID',
                    style: TextStyle(
                      fontSize: 14,
                      fontWeight: FontWeight.bold,
                      color: AppTheme.mainText,
                    ),
                  ),
                const SizedBox(height: 8),
                CustomTextField(
                  controller: _nationalIDController,
                  textType: TextInputType.emailAddress,
                  hintText: '10203040506070',
                  icon: Icons.email_outlined,
                  themeColor: widget.themeColor, 
                ),

                const SizedBox(height: 20),
                const Text(
                    'Password',
                    style: TextStyle(
                      fontSize: 14,
                      fontWeight: FontWeight.bold,
                      color: AppTheme.mainText,
                    ),
                  ),

                const SizedBox(height: 8),
                CustomTextField(
                  controller: _passwordController,
                  textType: TextInputType.text,
                  hintText: '••••••••',
                  icon: Icons.lock_outline,
                  themeColor: widget.themeColor,
                  isPassword: true,
                ),

                const SizedBox(height: 40),
                SizedBox(
                  width: 600,
                  child: ElevatedButton(
                    style: ElevatedButton.styleFrom(backgroundColor: widget.themeColor),
                    onPressed: () {
                      final id       = _nationalIDController.text.trim();
                      final password = _passwordController.text.trim();
                      _passwordController.clear();

                      // ADDED: empty field validation
                      if (id.isEmpty || password.isEmpty) {
                        ScaffoldMessenger.of(context).showSnackBar(
                          const SnackBar(
                            content: Text('Please fill in all fields'),
                            backgroundColor: AppTheme.subtitleText,
                          ),
                        );
                        return;
                      }

                      widget.onLogin(id, password);
                    },
                    child: const Text(
                      'Login',
                      style: TextStyle(fontSize: 16, fontWeight: FontWeight.bold, color: AppTheme.background),
                    ),
                  ),
                ),
                const SizedBox(height: 20),
                if(widget.isPassengerLogin)
                Center(
                    child : TextButton(
                    onPressed: () {
                        Navigator.push(
                          context,
                          MaterialPageRoute(
                            builder: (context) => Signup(
                              themeColor: widget.themeColor,
                            
                              onSignup: (name, nationalID, phone, email, password) {
                                final success = AirlineBridge.passengerSignup(
                                  name, nationalID, phone, email, password,
                                );
                                if (success) {
                                  AirlineBridge.saveAllData();  
                                  ScaffoldMessenger.of(context).showSnackBar(
                                    const SnackBar(
                                      content: Text('Account created! Please login.'),
                                      backgroundColor: AppTheme.subtitleText,
                                    ),
                                  );
                                  Navigator.pop(context);
                                } else {
                                  // ADDED: failure feedback
                                  ScaffoldMessenger.of(context).showSnackBar(
                                    const SnackBar(
                                      content: Text('Signup failed. ID may already exist.'),
                                      backgroundColor: AppTheme.subtitleText,
                                    ),
                                  );
                                }
                              },
                            ),
                          ),
                        );
                      },
                    child: Text(
                      'Don\'t have an account? Sign up',
                      style: TextStyle(color: widget.themeColor),
                    ),
                  )
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}