import 'package:final_proj/Services/ffi_bridge.dart';
import 'package:final_proj/utils/app_theme.dart';
import 'package:flutter/material.dart';

class AddAdminDialog extends StatefulWidget {
  final VoidCallback onAdminAdded;
  const AddAdminDialog({super.key, required this.onAdminAdded});

  @override
  State<AddAdminDialog> createState() => _AddAdminDialogState();
}

class _AddAdminDialogState extends State<AddAdminDialog> {
  final _nameCtrl  = TextEditingController();
  final _idCtrl    = TextEditingController();
  final _phoneCtrl = TextEditingController();
  final _emailCtrl = TextEditingController();

  @override
  void dispose() {
    _nameCtrl.dispose(); _idCtrl.dispose();
    _phoneCtrl.dispose(); _emailCtrl.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      backgroundColor: AppTheme.cardColor,
      title: const Text(
        "Add Admin",
        style: TextStyle(color: AppTheme.mainText, fontWeight: FontWeight.bold),
      ),
      content: SingleChildScrollView(
        child: Column(mainAxisSize: MainAxisSize.min, children: [
          _field(_nameCtrl,  "Full Name"),
          _field(_idCtrl,    "National ID"),
          _field(_phoneCtrl, "Phone Number", TextInputType.phone),
          _field(_emailCtrl, "Email",        TextInputType.emailAddress),
        ]),
      ),
      actions: [
        TextButton(
          onPressed: () => Navigator.pop(context),
          child: const Text("Cancel"),
        ),
        ElevatedButton(
          style: ElevatedButton.styleFrom(backgroundColor: AppTheme.adminAccent),
          onPressed: () {
            final name  = _nameCtrl.text.trim();
            final id    = _idCtrl.text.trim();
            final phone = _phoneCtrl.text.trim();
            final email = _emailCtrl.text.trim();

            if (name.isEmpty || id.isEmpty || phone.isEmpty || email.isEmpty) {
              ScaffoldMessenger.of(context).showSnackBar(const SnackBar(
                content: Text('Please fill in all fields.'),
                backgroundColor: AppTheme.subtitleText,
              ));
              return;
            }

            final success = AirlineBridge.addAdmin(name, id, phone, email);

            if (success) {AirlineBridge.saveAllData(); widget.onAdminAdded();}
            Navigator.pop(context);
            ScaffoldMessenger.of(context).showSnackBar(SnackBar(
              content: Text(success ? 'Admin added!' : 'Failed — ID may already exist.'),
              backgroundColor: AppTheme.subtitleText,
            ));
          },
          child: const Text("Add", style: TextStyle(color: Colors.white)),
        ),
      ],
    );
  }

  Widget _field(TextEditingController c, String label, [TextInputType? type]) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 10),
      child: TextField(
        controller: c,
        keyboardType: type,
        decoration: InputDecoration(
          labelText: label,
          border: const OutlineInputBorder(),
        ),
      ),
    );
  }
}