import 'package:final_proj/Services/ffi_bridge.dart';
import 'package:final_proj/utils/app_theme.dart';
import 'package:flutter/material.dart';

class CancelTicketDialog extends StatefulWidget {
  const CancelTicketDialog({super.key});

  @override
  State<CancelTicketDialog> createState() => _CancelTicketDialogState();
}

class _CancelTicketDialogState extends State<CancelTicketDialog> {
  final _ctrl = TextEditingController();

  @override
  void dispose() { _ctrl.dispose(); super.dispose(); }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      backgroundColor: AppTheme.cardColor,
      title: const Text("Cancel Ticket", style: TextStyle(color: AppTheme.mainText, fontWeight: FontWeight.bold)),
      content: TextField(
        controller: _ctrl,
        keyboardType: TextInputType.number,
        decoration: const InputDecoration(labelText: "Ticket ID", border: OutlineInputBorder()),
      ),
      actions: [
        TextButton(onPressed: () => Navigator.pop(context), child: const Text("Close")),
        ElevatedButton(
          style: ElevatedButton.styleFrom(backgroundColor: AppTheme.adminAccent),
          onPressed: () {
            final id = int.parse(_ctrl.text.trim().toString());
            final success = AirlineBridge.cancelTicket(id);
            if(success){AirlineBridge.saveAllData();}
            Navigator.pop(context);
            ScaffoldMessenger.of(context).showSnackBar(SnackBar(
              content: Text(success ? 'Ticket cancelled.' : 'Ticket not found.'),
              backgroundColor:AppTheme.subtitleText,
            ));
          },
          child: const Text("Cancel Ticket", style: TextStyle(color: Colors.white)),
        ),
      ],
    );
  }
}