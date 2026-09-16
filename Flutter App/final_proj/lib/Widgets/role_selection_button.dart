import 'package:flutter/material.dart';
import 'package:final_proj/utils/app_theme.dart';

class RoleSelectionButton extends StatelessWidget {
  final String title;
  final String subtitle;
  final IconData icon;
  final Color themeColor;
  final VoidCallback onTap;

  const RoleSelectionButton({
    super.key,
    required this.title,
    required this.subtitle,
    required this.icon,
    required this.themeColor,
    required this.onTap,
  });

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      width: 600, 
      child: OutlinedButton(
        onPressed: onTap,
        style: ButtonStyle(
          padding: WidgetStateProperty.all(const EdgeInsets.all(20)),
          backgroundColor: WidgetStateProperty.all(AppTheme.cardColor),
          
          shape: WidgetStateProperty.all(
            RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(20),
            ),
          ),
          
          side: WidgetStateProperty.resolveWith((states) {
            if (states.contains(WidgetState.hovered)) {
              return BorderSide(color: themeColor.withValues(alpha: 0.6), width: 4);
            }
            return BorderSide(color: themeColor.withValues(alpha: 0.3), width: 2);
          }),
        ),
        
        child: Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            CircleAvatar(
              radius: 30,
              backgroundColor: themeColor.withValues(alpha: 0.1),
              child: Icon(icon, size: 30, color: themeColor),
            ),
            const SizedBox(width: 20),
            Expanded(
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    title,
                    style: const TextStyle(
                      fontSize: 18,
                      fontWeight: FontWeight.bold,
                      color: AppTheme.mainText,
                    ),
                  ),
                  Text(
                    subtitle,
                    style: const TextStyle(
                      fontSize: 14,
                      color: AppTheme.subtitleText,
                    ),
                  ),
                ],
              ),
            ),
            Icon(Icons.arrow_forward_ios, color: AppTheme.subtitleText.withValues(alpha: 0.5), size: 16),
          ],
        ),
      )
    );
  }
}
