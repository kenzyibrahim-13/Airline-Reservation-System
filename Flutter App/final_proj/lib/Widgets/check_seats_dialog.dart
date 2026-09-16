import 'package:final_proj/Services/ffi_bridge.dart';
import 'package:final_proj/utils/app_theme.dart';
import 'package:flutter/material.dart';

class CheckSeatsDialog extends StatefulWidget {
  const CheckSeatsDialog({super.key});

  @override
  State<CheckSeatsDialog> createState() => _CheckSeatsDialogState();
}

class _CheckSeatsDialogState extends State<CheckSeatsDialog> {
  final _idCtrl    = TextEditingController();
  String _class    = 'Economy';
  List<String>? _seats;

  @override
  void dispose() { _idCtrl.dispose(); super.dispose(); }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      backgroundColor: AppTheme.cardColor,
      title: const Text("Available Seats", style: TextStyle(color: AppTheme.mainText, fontWeight: FontWeight.bold)),
      content: Column(mainAxisSize: MainAxisSize.min, children: [
        TextField(
          controller: _idCtrl,
          keyboardType: TextInputType.number,
          decoration: const InputDecoration(labelText: "Flight ID", border: OutlineInputBorder()),
        ),
        const SizedBox(height: 10),
        DropdownButtonFormField<String>(
          initialValue: _class,
          items: ["Economy", "Business"].map((c) => DropdownMenuItem(value: c, child: Text(c))).toList(),
          onChanged: (v) => setState(() { _class = v!; _seats = null; }),
          decoration: const InputDecoration(labelText: "Class", border: OutlineInputBorder()),
        ),
        if (_seats != null) ...[
          const SizedBox(height: 12),
          Text('${_seats!.length} seats available', style: const TextStyle(fontWeight: FontWeight.bold)),
          const SizedBox(height: 8),
          Wrap(
            spacing: 6, runSpacing: 6,
            children: _seats!.map((s) => Chip(label: Text(s, style: const TextStyle(fontSize: 11)))).toList(),
          ),
        ],
      ]),
      actions: [
        TextButton(onPressed: () => Navigator.pop(context), child: const Text("Close")),
        ElevatedButton(
          style: ElevatedButton.styleFrom(backgroundColor: AppTheme.adminAccent),
          onPressed: () {
            final id = int.tryParse(_idCtrl.text.trim()) ?? -1;
            final isBusiness = _class == 'Business';
            setState(() {
              _seats = AirlineBridge.getAvailableSeats(id, isBusiness);
            });
          },
          child: const Text("Check", style: TextStyle(color: Colors.white)),
        ),
      ],
    );
  }
}