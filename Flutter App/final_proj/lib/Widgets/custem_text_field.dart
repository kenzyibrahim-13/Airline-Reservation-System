import 'package:flutter/material.dart';
import 'package:final_proj/utils/app_theme.dart';

class CustomTextField extends StatefulWidget {
  final TextEditingController controller;
  final TextInputType textType; 
  final String hintText;
  final IconData icon;
  final Color themeColor;
  final bool isPassword;
  
  const CustomTextField({
    super.key,
    required this.controller,
    required this.textType,
    required this.hintText,
    required this.icon,
    required this.themeColor,
    this.isPassword = false
    });

  @override
  State<CustomTextField> createState() => _CustomTextFieldState();
}

class _CustomTextFieldState extends State<CustomTextField> {
  bool _obscurePassword = true;

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      width: 600, 
      child : TextField(
        controller: widget.controller,
        keyboardType: widget.textType,
        obscureText: widget.isPassword && _obscurePassword,
        decoration: InputDecoration(
          hintText: widget.hintText,
          hintStyle: TextStyle(color: AppTheme.subtitleText),
          prefixIcon: Icon(widget.icon, color: widget.themeColor),
          suffixIcon: widget.isPassword
              ? IconButton(
                  icon: Icon(
                    _obscurePassword ? Icons.visibility_off : Icons.visibility,
                    color: AppTheme.subtitleText,
                  ),
                  onPressed: () {
                    setState(() {
                      _obscurePassword = !_obscurePassword;
                    });
                  },
                )
              : null,
          filled: true,
          fillColor: AppTheme.cardColor,
          border: OutlineInputBorder(
            borderRadius: BorderRadius.circular(16),
            borderSide: BorderSide(color: widget.themeColor.withValues(alpha: 0.3)),
          ),
          enabledBorder: OutlineInputBorder(
            borderRadius: BorderRadius.circular(16),
            borderSide: BorderSide(color: widget.themeColor.withValues(alpha: 0.3)),
          ),
          focusedBorder: OutlineInputBorder(
            borderRadius: BorderRadius.circular(16),
            borderSide: BorderSide(color: widget.themeColor, width: 2),
          ),
        ),
      )
    );
  }
}