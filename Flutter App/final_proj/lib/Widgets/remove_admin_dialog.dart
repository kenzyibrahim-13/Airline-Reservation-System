import 'package:final_proj/Services/ffi_bridge.dart';
import 'package:final_proj/utils/app_theme.dart';
import 'package:flutter/material.dart';

class RemoveAdminDialog extends StatefulWidget {
  final VoidCallback onAdminRemoved;
  const RemoveAdminDialog({super.key, required this.onAdminRemoved});

  @override
  State<RemoveAdminDialog> createState() => _RemoveAdminDialogState();
}

class _RemoveAdminDialogState extends State<RemoveAdminDialog> {
  final _idCtrl = TextEditingController();

  @override
  void dispose() { _idCtrl.dispose(); super.dispose(); }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      backgroundColor: AppTheme.cardColor,
      title: const Text(
        "Remove Admin",
        style: TextStyle(color: AppTheme.mainText, fontWeight: FontWeight.bold),
      ),
      content: TextField(
        controller: _idCtrl,
        decoration: const InputDecoration(
          labelText: "Admin National ID",
          border: OutlineInputBorder(),
        ),
      ),
      actions: [
        TextButton(
          onPressed: () => Navigator.pop(context),
          child: const Text("Cancel"),
        ),
        ElevatedButton(
          style: ElevatedButton.styleFrom(backgroundColor: AppTheme.adminAccent),
          onPressed: () {
            final id = _idCtrl.text.trim();
            if (id.isEmpty) return;

            final success = AirlineBridge.removeAdmin(id);
            if (success) {AirlineBridge.saveAllData(); widget.onAdminRemoved();}
            Navigator.pop(context);
            ScaffoldMessenger.of(context).showSnackBar(SnackBar(
              content: Text(success ? 'Admin removed.' : 'Admin not found.'),
              backgroundColor: AppTheme.subtitleText,
            ));
          },
          child: const Text("Remove", style: TextStyle(color: Colors.white)),
        ),
      ],
    );
  }
}