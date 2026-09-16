import 'package:flutter/material.dart';
import 'package:final_proj/utils/app_theme.dart';

class CustomToggleButton extends StatelessWidget {
  final String label;
  final bool isActive;
  final Color themeColor;
  final VoidCallback onTap;

  const CustomToggleButton({
    super.key,
    required this.label,
    required this.isActive,
    required this.themeColor,
    required this.onTap,
  });

  @override
  Widget build(BuildContext context) {
    return Expanded(
      child: GestureDetector(
        onTap: onTap,
        child: Container(
          padding: const EdgeInsets.symmetric(vertical: 12),
          decoration: BoxDecoration(
            color: isActive ? themeColor : Colors.transparent,
            borderRadius: BorderRadius.circular(12),
          ),
          child: Center(
            child: Text(
              label,
              style: TextStyle(
                color: isActive ? Colors.white : AppTheme.subtitleText,
                fontWeight: FontWeight.bold,
              ),
            ),
          ),
        ),
      ),
    );
  }
}