import 'package:flutter/material.dart';

class Seat extends StatelessWidget {
  final String seatID;
  final bool isAvailable;
  final bool isSelected;
  final Color themeColor;
  final VoidCallback? onTap;
  
  const Seat({
    super.key,
    required this.seatID,
    required this.isAvailable,
    required this.isSelected,
    required this.themeColor,
    this.onTap
    });

  @override
  Widget build(BuildContext context) {

    Color seatColor;
    if (isSelected){
      seatColor = themeColor;
    }
    else if(isAvailable){
      seatColor = Colors.green.shade400;
    }
    else{
      seatColor = Colors.red.shade400;
    }

    Color borderColor;
    if (isSelected){
      borderColor = themeColor;
    }
    else if(isAvailable){
      borderColor = Colors.green.shade900;
    }
    else{
      borderColor = Colors.red.shade900;
    }

    return GestureDetector(
      onTap: isAvailable? onTap : null,
      child: Container(
        width: 36,
        height: 36,
        decoration: BoxDecoration(
          color: seatColor,
          borderRadius: BorderRadius.circular(6),
          border: Border.all(color: borderColor,width: 2)
        ),
        child: Center(
          child: Text(
            seatID,
            style: TextStyle(
              fontSize: 8,
              color: isSelected ? Colors.white : Colors.black
            ),
          ),
        ) 
      )
    );
  }
}
