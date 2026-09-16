import 'package:final_proj/Services/ffi_bridge.dart';
import 'package:flutter/material.dart';
import 'package:final_proj/utils/app_theme.dart';

class TicketCard extends StatelessWidget {
  final int ticketID;
  final int flightID;
  final String seatNumber;
  final bool isBusiness;
  final double totalWeight;
  final bool hasExtraFees;
  final double extraFee;
  final double totalPrice;
  final Color themeColor;
  final String nationalID;
  final VoidCallback onCancel;

  const TicketCard({
    super.key,
    required this.ticketID,
    required this.flightID,
    required this.seatNumber,
    required this.isBusiness,
    required this.totalWeight,
    required this.hasExtraFees,
    required this.extraFee,
    required this.totalPrice,
    required this.themeColor,
    required this.onCancel,
    required this.nationalID
  });

  @override
  Widget build(BuildContext context) {
    return Container(
      margin: const EdgeInsets.only(bottom: 16),
      decoration: BoxDecoration(
        color: AppTheme.cardColor,
        borderRadius: BorderRadius.circular(20),
        border: Border.all(color: themeColor.withValues(alpha: 0.5)),
        
      ),
      child: Padding(
        padding: const EdgeInsets.all(20),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [

            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Text(
                  'Ticket #$ticketID',
                  style: TextStyle(
                    fontSize: 18,
                    fontWeight: FontWeight.bold,
                    color: themeColor,
                  ),
                ),
                Container(
                  padding: const EdgeInsets.symmetric(
                    horizontal: 20,
                    vertical: 10,
                  ),
                  decoration: BoxDecoration(
                    color: themeColor.withValues(alpha: 0.1),
                    borderRadius: BorderRadius.circular(20),
                    border: Border.all(color: themeColor.withValues(alpha: 0.4)),
                  ),
                  child: Text(
                    isBusiness ? 'Business' : 'Economy',
                    style: TextStyle(
                      fontSize: 12,
                      fontWeight: FontWeight.bold,
                      color: themeColor,
                    ),
                  ),
                ),
              ],
            ),

            const SizedBox(height: 16),
            Divider(color: themeColor.withValues(alpha: 0.2)),
            const SizedBox(height: 12),

            Row(
              children: [
                _infoChip(Icons.flight, 'Flight #$flightID', themeColor),
                const SizedBox(width: 16),
                _infoChip(Icons.event_seat_outlined, 'Seat $seatNumber', themeColor),
                const SizedBox(width: 16),
                _infoChip(Icons.person, '#ID: $nationalID', themeColor),
              ],
            ),

            const SizedBox(height: 12),

            _infoChip(
              Icons.luggage_outlined,
              'Total Weight: ${totalWeight.toStringAsFixed(1)} kg',
              themeColor,
            ),

            if (hasExtraFees) ...[
              const SizedBox(height: 8),
              Container(
                padding: const EdgeInsets.symmetric(
                  horizontal: 12,
                  vertical: 8,
                ),
                decoration: BoxDecoration(
                  color: Colors.orange.withValues(alpha: 0.1),
                  borderRadius: BorderRadius.circular(10),
                  border: Border.all(color: Colors.orange.withValues(alpha: 0.4)),
                ),
                child: Row(
                  children: [
                    const Icon(Icons.warning_amber_outlined,
                        color: Colors.orange, size: 16),
                    const SizedBox(width: 6),
                    Text(
                      'Extra Luggage Fee: \$${extraFee.toStringAsFixed(2)}',
                      style: const TextStyle(
                        fontSize: 13,
                        color: Colors.orange,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                  ],
                ),
              ),
            ],

            const SizedBox(height: 16),
            Divider(color: themeColor.withValues(alpha: 0.2)),
            const SizedBox(height: 12),

            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                TextButton.icon(
                  onPressed: () {
                    bool success = AirlineBridge.cancelTicket(ticketID);
                    if (success){
                      AirlineBridge.saveAllData();
                      onCancel();
                    }
                  },
                  icon: const Icon(Icons.cancel_outlined, color: Colors.redAccent, size: 18),
                  label: const Text(
                    'Cancel Ticket',
                    style: TextStyle(color: Colors.redAccent, fontWeight: FontWeight.bold),
                  ),
                ),
                Column(
                  crossAxisAlignment: CrossAxisAlignment.end,
                  children: [
                    const Text('Total Price', style: TextStyle(fontSize: 12, color: AppTheme.subtitleText)),
                    Text(
                      '\$${totalPrice.toStringAsFixed(2)}',
                      style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold, color: themeColor),
                    ),
                  ],
                ),
              ],
            ),

          ],
        ),
      ),
    );
  }

  Widget _infoChip(IconData icon, String label, Color color) {
    return Row(
      children: [
        Icon(icon, size: 14, color: color),
        const SizedBox(width: 4),
        Text(
          label,
          style: const TextStyle(
            fontSize: 13,
            color: AppTheme.subtitleText,
          ),
        ),
      ],
    );
  }
}