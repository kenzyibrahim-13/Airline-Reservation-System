import 'package:flutter/material.dart';
import 'package:final_proj/Widgets/seat.dart';
import 'package:final_proj/utils/app_theme.dart';

class RowBuilder {
  static List<Widget> buildRows({
    required List<String> seats,
    required List<String> takenSeats,
    required String? selectedSeat,
    required Color themeColor,
    required void Function(String seatID) onSeatTap
  }){
    List<Widget> rows = [];

    for (int i=0; i < seats.length; i+=6){
      String rowNumber = seats[i].replaceAll(RegExp(r'[A-F]'), '');

      rows.add(
        Padding(
          padding: const EdgeInsets.only(bottom: 6),
          child: Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              SizedBox(
                width: 28,
                child: Text(
                  rowNumber,
                  style: const TextStyle(
                    fontSize: 11,
                    color: AppTheme.subtitleText
                  ),
                  textAlign: TextAlign.center,
                ),
              ),

              for (int j=0; j<3 ;j++)
                Padding(
                  padding: const EdgeInsets.only(right: 4),
                  child: Seat(
                    seatID: seats[i+j],
                    isAvailable: !takenSeats.contains(seats[i+j]),
                    isSelected: selectedSeat == seats[i+j],
                    themeColor: themeColor,
                    onTap: () => onSeatTap(seats[i+j]),
                  )
                ),

              const SizedBox(width: 40),

              for (int j=3; j<6 ;j++)
                Padding(
                  padding: const EdgeInsets.only(right: 4),
                  child: Seat(
                    seatID: seats[i+j],
                    isAvailable: !takenSeats.contains(seats[i+j]),
                    isSelected: selectedSeat == seats[i+j],
                    themeColor: themeColor,
                    onTap: () => onSeatTap(seats[i+j]),
                  )
                ),

            ],
          )
        )
      );
    }
    return rows;
  }
}
