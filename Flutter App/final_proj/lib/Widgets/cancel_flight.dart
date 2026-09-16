import 'package:final_proj/Services/ffi_bridge.dart';
import 'package:final_proj/utils/app_theme.dart';
import 'package:flutter/material.dart';

class CancelFlightDialog extends StatefulWidget {
  final VoidCallback onFlightCancelled;
  const CancelFlightDialog({super.key, required this.onFlightCancelled});

  @override
  State<CancelFlightDialog> createState() => _CancelFlightDialogState();
}

class _CancelFlightDialogState extends State<CancelFlightDialog> {
  final _ctrl = TextEditingController();

  @override
  void dispose() { _ctrl.dispose(); super.dispose(); }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      backgroundColor: AppTheme.cardColor,
      title: const Text("Cancel Flight", style: TextStyle(color: AppTheme.mainText, fontWeight: FontWeight.bold)),
      content: TextField(
        controller: _ctrl,
        keyboardType: TextInputType.number,
        decoration: const InputDecoration(labelText: "Flight ID", border: OutlineInputBorder()),
      ),
      actions: [
        TextButton(onPressed: () => Navigator.pop(context), child: const Text("Go Back")),
        ElevatedButton(
          style: ElevatedButton.styleFrom(backgroundColor: AppTheme.adminAccent),
          onPressed: () {
            final id = int.tryParse(_ctrl.text.trim()) ?? -1;
            final success = AirlineBridge.adminCancelFlight(id);
            if (success) {AirlineBridge.saveAllData(); widget.onFlightCancelled();}
            Navigator.pop(context);
            ScaffoldMessenger.of(context).showSnackBar(SnackBar(
              content: Text(success ? 'Flight cancelled.' : 'Flight not found.'),
              backgroundColor: AppTheme.subtitleText,
            ));
          },
          child: const Text("Confirm", style: TextStyle(color: Colors.white)),
        ),
      ],
    );
  }
}