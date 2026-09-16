import 'package:flutter/material.dart';
import 'package:final_proj/Widgets/custem_text_field.dart';
import 'package:final_proj/utils/app_theme.dart';

class Signup extends StatefulWidget {
  final Color themeColor;
  final Function (
    String name, 
    String nationalID,
    String phone, 
    String email, 
    String password) onSignup;
  
  const Signup({
    super.key,
    required this.themeColor,
    required this.onSignup
    });

  @override
  State<Signup> createState() => _SignupState();
}

class _SignupState extends State<Signup> {
  final TextEditingController _nameController = TextEditingController();
  final TextEditingController _nationalIDController = TextEditingController();
  final TextEditingController _phoneController = TextEditingController();
  final TextEditingController _emailController = TextEditingController();
  final TextEditingController _passwordController = TextEditingController();
  
  @override
  void dispose() {
    _nameController.dispose();
    _nationalIDController.dispose();
    _phoneController.dispose();
    _emailController.dispose();
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
        child: SingleChildScrollView(
          padding: const EdgeInsets.all(24.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.center,
            children: [
              Center(
                child: Container(
                  padding: const EdgeInsets.all(20),
                  decoration: BoxDecoration(
                    color: widget.themeColor.withValues(alpha: 0.1),
                    shape: BoxShape.circle,
                  ),
                  child: Icon(Icons.person_add, size: 60, color: widget.themeColor),
                ),
              ),

              const SizedBox(height: 30),

              Center(
                child: Text(
                  'Create Account',
                  style: TextStyle(
                    fontSize: 28,
                    fontWeight: FontWeight.bold,
                    color: AppTheme.mainText,
                  ),
                ),
              ),
              const SizedBox(height: 8),
              const Center(
                child: Text(
                  'Fill in your details to get started',
                  style: TextStyle(fontSize: 16, color: AppTheme.subtitleText),
                ),
              ),

              const SizedBox(height: 40),

              const Text('Full Name', style: TextStyle(fontSize: 14, fontWeight: FontWeight.bold, color: AppTheme.mainText)),
              const SizedBox(height: 8),
              CustomTextField(
                controller: _nameController,
                textType: TextInputType.name,
                hintText: 'Enter your full name',
                icon: Icons.person_outline,
                themeColor: widget.themeColor,
              ),

              const SizedBox(height: 20),

              const Text('National ID', style: TextStyle(fontSize: 14, fontWeight: FontWeight.bold, color: AppTheme.mainText)),
              const SizedBox(height: 8),
              CustomTextField(
                controller: _nationalIDController,
                textType: TextInputType.number,
                hintText: '1234567890',
                icon: Icons.badge_outlined,
                themeColor: widget.themeColor,
              ),

              const SizedBox(height: 20),

              const Text('Phone Number', style: TextStyle(fontSize: 14, fontWeight: FontWeight.bold, color: AppTheme.mainText)),
              const SizedBox(height: 8),
              CustomTextField(
                controller: _phoneController,
                textType: TextInputType.phone,
                hintText: '+20 1234567890',
                icon: Icons.phone_outlined,
                themeColor: widget.themeColor,
              ),

              const SizedBox(height: 20),

              const Text('Email', style: TextStyle(fontSize: 14, fontWeight: FontWeight.bold, color: AppTheme.mainText)),
              const SizedBox(height: 8),
              CustomTextField(
                controller: _emailController,
                textType: TextInputType.emailAddress,
                hintText: '1234@email.com',
                icon: Icons.email_outlined,
                themeColor: widget.themeColor,
              ),

              const SizedBox(height: 20),

              // Password
              const Text('Password', style: TextStyle(fontSize: 14, fontWeight: FontWeight.bold, color: AppTheme.mainText)),
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

              // Signup Button
              SizedBox(
                width: 600,
                child: ElevatedButton(
                  onPressed: () {
                    final name       = _nameController.text.trim();
                    final nationalID = _nationalIDController.text.trim();
                    final phone      = _phoneController.text.trim();
                    final email      = _emailController.text.trim();
                    final password   = _passwordController.text.trim();

                    if (name.isEmpty || nationalID.isEmpty || phone.isEmpty || email.isEmpty || password.isEmpty) {
                      ScaffoldMessenger.of(context).showSnackBar(
                        const SnackBar(
                          content: Text('Please fill in all fields'),
                          backgroundColor: AppTheme.subtitleText,
                        ),
                      );
                      return;
                    }

                    if (password.length < 6) {
                      ScaffoldMessenger.of(context).showSnackBar(
                        const SnackBar(
                          content: Text('Password must be at least 6 characters'),
                          backgroundColor: AppTheme.subtitleText,
                        ),
                      );
                      return;
                    }

                    widget.onSignup(name, nationalID, phone, email, password);
                  },
                  style: ElevatedButton.styleFrom(
                    backgroundColor: widget.themeColor,
                    foregroundColor: Colors.white,
                    padding: const EdgeInsets.all(18),
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(16),
                    ),
                  ),
                  child: const Text(
                    'Create Account',
                    style: TextStyle(fontSize: 16, fontWeight: FontWeight.bold),
                  ),
                ),
              ),

              const SizedBox(height: 20),

              Center(
                child: TextButton(
                  onPressed: () => Navigator.pop(context), // ← goes back to login
                  child: Text(
                    'Already have an account? Login',
                    style: TextStyle(color: widget.themeColor),
                  ),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
