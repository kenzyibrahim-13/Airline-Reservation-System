import 'package:final_proj/Services/ffi_bridge.dart';
import 'package:final_proj/utils/app_theme.dart';
import 'package:flutter/material.dart';

class EditUserDialog extends StatefulWidget {
  const EditUserDialog({super.key});

  @override
  State<EditUserDialog> createState() => _EditUserDialogState();
}

class _EditUserDialogState extends State<EditUserDialog> {
  final _idCtrl    = TextEditingController();
  final _valCtrl   = TextEditingController();
  String _field    = 'Email';

  @override
  void dispose() { _idCtrl.dispose(); _valCtrl.dispose(); super.dispose(); }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      backgroundColor: AppTheme.cardColor,
      title: const Text("Edit Passenger", style: TextStyle(color: AppTheme.mainText, fontWeight: FontWeight.bold)),
      content: Column(mainAxisSize: MainAxisSize.min, children: [
        TextField(controller: _idCtrl,  decoration: const InputDecoration(labelText: "National ID",    border: OutlineInputBorder())),
        const SizedBox(height: 10),
        DropdownButtonFormField<String>(
          initialValue: _field,
          items: ["Email", "Phone"].map((v) => DropdownMenuItem(value: v, child: Text(v))).toList(),
          onChanged: (v) => setState(() => _field = v!),
          decoration: const InputDecoration(labelText: "Field", border: OutlineInputBorder()),
        ),
        const SizedBox(height: 10),
        TextField(controller: _valCtrl, decoration: const InputDecoration(labelText: "New Value",      border: OutlineInputBorder())),
      ]),
      actions: [
        TextButton(onPressed: () => Navigator.pop(context), child: const Text("Cancel")),
        ElevatedButton(
          style: ElevatedButton.styleFrom(backgroundColor: AppTheme.adminAccent),
          onPressed: () {
            final id  = _idCtrl.text.trim();
            final val = _valCtrl.text.trim();
            final success = _field == 'Email'
                ? AirlineBridge.editPassengerEmail(id, val)
                : AirlineBridge.editPassengerPhone(id, val);
            AirlineBridge.saveAllData();
            Navigator.pop(context);
            ScaffoldMessenger.of(context).showSnackBar(SnackBar(
              content: Text(success ? '$_field updated!' : 'Passenger not found.'),
              backgroundColor: AppTheme.subtitleText,
            ));
          },
          child: const Text("Update", style: TextStyle(color: Colors.white)),
        ),
      ],
    );
  }
}