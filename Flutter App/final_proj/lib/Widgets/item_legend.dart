import 'package:flutter/material.dart';
import 'package:final_proj/utils/app_theme.dart';

class LegendItem extends StatelessWidget {
  final Color fill;
  final Color border;
  final String label;

  const LegendItem({
    super.key,
    required this.fill,
    required this.border,
    required this.label,
  });

  @override
  Widget build(BuildContext context) {
    return Row(
      children: [
        Container(
          width: 16,
          height: 16,
          decoration: BoxDecoration(
            color: fill,
            borderRadius: BorderRadius.circular(4),
            border: Border.all(color: border),
          ),
        ),
        const SizedBox(width: 4),
        Text(
          label,
          style: const TextStyle(fontSize: 12, color: AppTheme.subtitleText),
        ),
      ],
    );
  }
}