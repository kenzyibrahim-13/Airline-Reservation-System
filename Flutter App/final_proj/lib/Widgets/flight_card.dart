import 'package:final_proj/Screens/seat_picker.dart';
import 'package:final_proj/Services/ffi_bridge.dart';
import 'package:final_proj/utils/app_theme.dart';
import 'package:flutter/material.dart';

class FlightCard extends StatefulWidget{

  final int flightID;
  final String passengerID;
  final String origin;
  final String destination;
  final String date;
  final String time;
  final double price;
  final bool isFull;
  final Color themeColor;
  final VoidCallback onBooked;
  final bool isAdmin;

  const FlightCard({
    super.key,
    required this.flightID,
    required this.passengerID,
    required this.origin,
    required this.destination,
    required this.date,
    required this.time,
    required this.price,
    required this.isFull,
    required this.themeColor,
    required this.onBooked,
    this.isAdmin = false
  });
  
  @override
  State<FlightCard> createState()=> _FlightCardState();


}



class _FlightCardState extends State<FlightCard> {
  List<String> _takenSeats = [];


  @override
  void initState(){
    super.initState();
    _refreshTakenSeats();
  }

  void _refreshTakenSeats(){
    final takenEconomy = AirlineBridge.getTakenSeats(widget.flightID, false);
    final takenBusiness = AirlineBridge.getTakenSeats(widget.flightID, true);
    setState(() {
      _takenSeats = [...takenEconomy, ...takenBusiness];
    });
  }

  @override
  void didUpdateWidget(FlightCard oldWidget){
    super.didUpdateWidget(oldWidget);
    _refreshTakenSeats();
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      margin: const EdgeInsets.only(bottom: 16),
      decoration: BoxDecoration(
        color: AppTheme.cardColor,
        borderRadius: BorderRadius.circular(20),
        border: Border.all(color: widget.themeColor.withValues(alpha: 0.3)),
        
      ),
      child: Padding(
        padding: const EdgeInsets.all(20),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [

            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [

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

              ],
            ),

            const SizedBox(height: 16),
            Divider(color: widget.themeColor.withValues(alpha: 0.6)),
            const SizedBox(height: 12),

            Row(
              children: [
                _infoChip(Icons.calendar_today_outlined, widget.date, widget.themeColor),
                const SizedBox(width: 12),
                _infoChip(Icons.access_time, widget.time, widget.themeColor),
                const SizedBox(width: 12),
                _infoChip(Icons.tag, "ID: ${widget.flightID.toString()}", widget.themeColor)
              ],
            ),

            const SizedBox(height: 16),

            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const Text(
                      'Price',
                      style: TextStyle(
                        fontSize: 12,
                        color: AppTheme.subtitleText,
                      ),
                    ),
                    Text(
                      '\$${widget.price.toStringAsFixed(2)}',
                      style: TextStyle(
                        fontSize: 22,
                        fontWeight: FontWeight.bold,
                        color: widget.themeColor,
                      ),
                    ),
                  ],
                ),
                widget.isAdmin?
                Text(
                  'View Only', 
                )

                : ElevatedButton(
                  onPressed: widget.isFull?
                      null
                      : () {
                          showModalBottomSheet(
                            context: context,
                            isScrollControlled: true,
                            backgroundColor: Colors.transparent,
                            builder: (context) => SeatPicker(
                              origin: widget.origin,
                              destination: widget.destination,
                              themeColor: widget.themeColor,
                              takenSeats: _takenSeats,
                              onConfirm: (seatID, seatClass, weight) {
                                Navigator.pop(context);
                                bool isBusiness = (seatClass == 'Business');
                                int ticketID = AirlineBridge.bookTicket(widget.passengerID, widget.flightID, seatID, isBusiness);

                                if (ticketID != -1){
                                  if (weight > 0){
                                    AirlineBridge.addLuggage(ticketID, weight, false);
                                  }
                                  _refreshTakenSeats();
                                  widget.onBooked();
                                }
                                AirlineBridge.saveAllData();
                                ScaffoldMessenger.of(context).showSnackBar(
                                  SnackBar(
                                    content: Text(
                                      'Seat $seatID ($seatClass) booked with ${weight}kg luggage!',
                                    ),
                                    backgroundColor: AppTheme.subtitleText,
                                  ),
                                );
                              },
                            ),
                          );
                        },
                  style: ElevatedButton.styleFrom(
                    backgroundColor: widget.isFull ? AppTheme.subtitleText : widget.themeColor,
                    foregroundColor: Colors.white,
                    disabledBackgroundColor:
                        AppTheme.subtitleText.withValues(alpha: 0.3),
                    padding: const EdgeInsets.symmetric(
                      horizontal: 24,
                      vertical: 14,
                    ),
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(14),
                    ),
                  ),
                  child: Text(
                    widget.isFull ? 'Full' : 'Book Now',
                    style: const TextStyle(
                      fontSize: 14,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
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