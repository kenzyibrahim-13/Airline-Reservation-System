import 'package:flutter/material.dart';
import 'package:final_proj/Services/seat_generator.dart';
import 'package:final_proj/Widgets/custom_toggle_button.dart';
import 'package:final_proj/Widgets/item_legend.dart';
import 'package:final_proj/Widgets/row_builder.dart';
import 'package:final_proj/utils/app_theme.dart';
import 'package:flutter/services.dart';

class SeatPicker extends StatefulWidget {
  final String origin;
  final String destination;
  final Color themeColor;
  final List<String> takenSeats;
  final void Function(String seatID,String seatClass, double weight) onConfirm;
  const SeatPicker({
    required this.origin,
    required this.destination,
    required this.themeColor,
    required this.takenSeats,
    required this.onConfirm,
    super.key
    });

  @override
  State<SeatPicker> createState() => _SeatPickerState();
}

class _SeatPickerState extends State<SeatPicker> {
  String? _selectedSeat;
  bool _isBusinessClass = false;
  final TextEditingController _weightController = TextEditingController(text: '');

  @override
  void dispose() {
    _weightController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    List<String> seats = SeatGenerator.generateSeats(_isBusinessClass);

    return Container(
      height: MediaQuery.of(context).size.height * 0.8,
      decoration: BoxDecoration(
        color: AppTheme.background,
        borderRadius: BorderRadius.circular(24)
      ),

      child: Column(
        children: [
          const SizedBox(height: 30),

          Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                const SizedBox(width: 12),
                Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Text(
                      widget.origin,
                      style: TextStyle(
                        fontSize: 20,
                        fontWeight: FontWeight.bold,
                        color: widget.themeColor,
                      ),
                    ),
                    const Text(
                      'Origin',
                      style: TextStyle(
                        fontSize: 12,
                        color: AppTheme.subtitleText,
                      ),
                    ),
                  ],
                ),

                Icon(Icons.arrow_forward, color: widget.themeColor, size: 24),

                Column(
                  crossAxisAlignment: CrossAxisAlignment.end,
                  children: [
                    Text(
                      widget.destination,
                      style: TextStyle(
                        fontSize: 20,
                        fontWeight: FontWeight.bold,
                        color: widget.themeColor,
                      ),
                    ),
                    const Text(
                      'Destination',
                      style: TextStyle(
                        fontSize: 12,
                        color: AppTheme.subtitleText,
                      ),
                    ),
                  ],
                ),
                const SizedBox(width: 12),

              ],
            ),

          const SizedBox(height: 4),

          const Text(
            'Select Your Seat',
            style: TextStyle(
              fontSize: 14,
              fontWeight: FontWeight.normal,
              color: AppTheme.subtitleText
            )
          ),

          const SizedBox(height: 16),

          Container(
            margin: const EdgeInsets.symmetric(horizontal: 24),
            decoration: BoxDecoration(
              color: AppTheme.cardColor,
              borderRadius: BorderRadius.circular(12),
              border: Border.all(color: widget.themeColor.withValues(alpha: 0.3))
            ),
            child: Row(
              children: [
                CustomToggleButton(
                  label: 'Economy',
                  isActive: !_isBusinessClass,
                  themeColor: widget.themeColor,
                  onTap: () => setState(() {
                    _isBusinessClass = false;
                    _selectedSeat = null;
                  }),
                ),
                CustomToggleButton(
                  label: 'Business',
                  isActive: _isBusinessClass,
                  themeColor: widget.themeColor,
                  onTap: () => setState(() {
                    _isBusinessClass = true;
                    _selectedSeat = null;
                  }),
                ),
              ]
            ),
          ),

          const SizedBox(height: 16),

          Padding(
            padding: const EdgeInsets.symmetric(horizontal: 24),
            child: TextField(
              controller: _weightController,
              keyboardType: const TextInputType.numberWithOptions(decimal: true),
              inputFormatters: <TextInputFormatter>[FilteringTextInputFormatter.allow(RegExp(r'^\d+\.?\d*'))],
              style: const TextStyle(color: AppTheme.mainText),
              decoration: InputDecoration(
                labelText: 'Luggage Weight (kg)',
                hintText: 'Enter a number',
                labelStyle: const TextStyle(color: AppTheme.subtitleText),
                prefixIcon: Icon(Icons.luggage, color: widget.themeColor),
                enabledBorder: OutlineInputBorder(
                  borderRadius: BorderRadius.circular(12),
                  borderSide: BorderSide(color: widget.themeColor.withValues(alpha: 0.3)),
                ),
                focusedBorder: OutlineInputBorder(
                  borderRadius: BorderRadius.circular(12),
                  borderSide: BorderSide(color: widget.themeColor),
                ),
              ),
            ),
          ),

          const SizedBox(height: 16),

          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              LegendItem(
                fill: AppTheme.seatAvailable.withValues(alpha: 0.3), 
                border: AppTheme.seatAvailable, 
                label: 'Available'
              ),

              const SizedBox(width: 16),

              LegendItem(
                fill: AppTheme.seatTaken.withValues(alpha: 0.3), 
                border: AppTheme.seatTaken, 
                label: 'Taken'
              ),

              const SizedBox(width: 16),

              LegendItem(
                fill: widget.themeColor, 
                border: widget.themeColor, 
                label: 'Selected'
              )
            ],
          ),

          const SizedBox(height: 16),

          Padding(
            padding: const EdgeInsets.symmetric(horizontal: 24),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                
                const SizedBox(width: 28),

                for (String col in ['A','B','C','','D','E','F'])
                  col.isEmpty?
                  const SizedBox(width: 16)
                  : SizedBox(
                    width: 36,
                    child: Center(
                      child: Text(
                        col,
                        style: const TextStyle(
                          fontSize: 12,
                          fontWeight: FontWeight.bold,
                          color: AppTheme.subtitleText,
                        ),
                      ),
                    )
                  )

              ],
            ), 
          ),

          const SizedBox(height: 8),

          Expanded(
            child: SingleChildScrollView(
              padding: const EdgeInsets.symmetric(horizontal: 24),
              child: Column(
                children: RowBuilder.buildRows(
                  seats: seats, 
                  takenSeats: widget.takenSeats, 
                  selectedSeat: _selectedSeat, 
                  themeColor: widget.themeColor, 
                  onSeatTap: (seatID) => setState(() {
                    _selectedSeat = seatID;
                  }),
                )
              ),
            ),
          ),

          Padding(
            padding: const EdgeInsets.all(24),
            child: SizedBox(
              width: double.infinity,
              child: ElevatedButton(
                onPressed: _selectedSeat == null?
                null
                : () {
                  double weight = double.tryParse(_weightController.text) ?? 0.0; 
                  widget.onConfirm(
                        _selectedSeat!,
                        _isBusinessClass ? 'Business' : 'Economy',
                        weight
                      );
                }, 
                style: ElevatedButton.styleFrom(
                  backgroundColor: widget.themeColor,
                  foregroundColor: AppTheme.cardColor,
                  disabledBackgroundColor: AppTheme.subtitleText.withValues(alpha: 0.3),
                  padding: const EdgeInsets.all(18),
                  shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16))
                ),
                child: Text(
                  _selectedSeat == null?
                  'Select a Seat'
                  : 'Confirm $_selectedSeat',
                  style: const TextStyle(
                    fontSize: 16,
                    fontWeight: FontWeight.bold
                  ),
                ),
              )
            )
          )
        ],
      ),
    );
  }
}