class SeatGenerator {
  static List<String> generateSeats(bool isBusinessClass) {
    List<String> seats = [];
    int startRow = isBusinessClass ? 1 : 6;
    int endRow = isBusinessClass ? 5 : 30;

    for (int row = startRow; row <= endRow; row++) {
      for (String col in ['A', 'B', 'C', 'D', 'E', 'F']) {
        seats.add('$row$col');
      }
    }
    return seats;
  }
}