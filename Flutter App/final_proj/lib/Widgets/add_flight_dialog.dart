import 'package:final_proj/Services/ffi_bridge.dart';
import 'package:final_proj/utils/app_theme.dart';
import 'package:flutter/material.dart';

class AddFlightDialog extends StatefulWidget {
  final VoidCallback onFlightAdded;
  const AddFlightDialog({super.key, required this.onFlightAdded});

  @override
  State<AddFlightDialog> createState() => _AddFlightDialogState();
}

class _AddFlightDialogState extends State<AddFlightDialog> {
  final _idCtrl       = TextEditingController();
  final _fromCtrl     = TextEditingController();
  final _toCtrl       = TextEditingController();
  final _priceCtrl    = TextEditingController();
  final _dateCtrl     = TextEditingController();
  final _timeCtrl     = TextEditingController();

  @override
  void dispose() {
    _idCtrl.dispose(); _fromCtrl.dispose(); _toCtrl.dispose();
    _priceCtrl.dispose(); _dateCtrl.dispose(); _timeCtrl.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      backgroundColor: AppTheme.cardColor,
      title: const Text("Add Flight", style: TextStyle(color: AppTheme.mainText, fontWeight: FontWeight.bold)),
      content: SingleChildScrollView(
        child: Column(children: [
          _field(_idCtrl,    "Flight ID",    TextInputType.number),
          _field(_fromCtrl,  "Origin"),
          _field(_toCtrl,    "Destination"),
          _field(_priceCtrl, "Price",        TextInputType.number),
          _field(_dateCtrl,  "Date (YYYY-MM-DD)"),
          _field(_timeCtrl,  "Time (HH:MM)"),
        ]),
      ),
      actions: [
        TextButton(onPressed: () => Navigator.pop(context), child: const Text("Cancel")),
        ElevatedButton(
          style: ElevatedButton.styleFrom(backgroundColor: AppTheme.adminAccent),
          onPressed: () {
            final success = AirlineBridge.adminAddFlight(
              _idCtrl.text.trim(),
              _fromCtrl.text.trim(),
              _toCtrl.text.trim(),
              180,
              double.tryParse(_priceCtrl.text.trim()) ?? 0,
              _dateCtrl.text.trim(),
              _timeCtrl.text.trim(),
            );
            
            if (success) {AirlineBridge.saveAllData(); widget.onFlightAdded();}
            Navigator.pop(context);
            ScaffoldMessenger.of(context).showSnackBar(SnackBar(
              content: Text(success ? 'Flight added!' : 'Failed to add flight.'),
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
        decoration: InputDecoration(labelText: label, border: const OutlineInputBorder()),
      ),
    );
  }
}