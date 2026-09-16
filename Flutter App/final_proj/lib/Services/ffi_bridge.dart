import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:final_proj/Services/seat_generator.dart';
import 'package:final_proj/services/AirlineBridgeBindings.dart';

// single instance of the generated bindings
final AirlineBridgeBindings _bindings = AirlineBridgeBindings(
  DynamicLibrary.open('libairline_bridge.dll'),
);

// helper
String _readString(Pointer<Char> ptr) => ptr.cast<Utf8>().toDartString();

class AirlineBridge {

  // --- Auth ---
  static bool passengerSignup(String name, String id, String phone, String email, String pass) {
    final n = name.toNativeUtf8().cast<Char>(); final i = id.toNativeUtf8().cast<Char>();
    final ph = phone.toNativeUtf8().cast<Char>(); final e = email.toNativeUtf8().cast<Char>();
    final p = pass.toNativeUtf8().cast<Char>();
    final result = _bindings.passenger_signup(n, i, ph, e, p);
    malloc.free(n); malloc.free(i); malloc.free(ph); malloc.free(e); malloc.free(p);
    return result == 1;
  }

  static bool passengerLogin(String id, String pass) {
    final i = id.toNativeUtf8().cast<Char>();
    final p = pass.toNativeUtf8().cast<Char>();
    final result = _bindings.passenger_login(i, p);
    malloc.free(i); malloc.free(p);
    return result == 1;
  }

  static bool adminLogin(String id, String pass) {
    final i = id.toNativeUtf8().cast<Char>();
    final p = pass.toNativeUtf8().cast<Char>();
    final result = _bindings.admin_login(i, p);
    malloc.free(i); malloc.free(p);
    return result == 1;
  }

  static bool superAdminLogin(String id, String pass) {
    final i = id.toNativeUtf8().cast<Char>();
    final p = pass.toNativeUtf8().cast<Char>();
    final result = _bindings.super_admin_login(i, p);
    malloc.free(i); 
    malloc.free(p);
    return result == 1;
  }

  // --- Flights ---
  static bool adminAddFlight(String id, String from, String to, int cap, double price, String date, String time) {
    final i = id.toNativeUtf8().cast<Char>(); final f = from.toNativeUtf8().cast<Char>();
    final t = to.toNativeUtf8().cast<Char>(); final d = date.toNativeUtf8().cast<Char>();
    final tm = time.toNativeUtf8().cast<Char>();
    final result = _bindings.admin_add_flight(i, f, t, cap, price, d, tm);
    malloc.free(i); malloc.free(f); malloc.free(t); malloc.free(d); malloc.free(tm);
    return result == 1;
  }

  static bool adminCancelFlight(int flightID) => _bindings.admin_cancel_flight(flightID) == 1;

  static List<Map<String, dynamic>> getAllFlights() {
    final count = _bindings.get_flight_count();
    final List<Map<String, dynamic>> flights = [];
    for (int idx = 0; idx < count; idx++) {
      final id     = calloc<Char>(256); final from  = calloc<Char>(256);
      final to     = calloc<Char>(256); final date  = calloc<Char>(256);
      final time   = calloc<Char>(256); final price = calloc<Double>();
      final isFull = calloc<Int>();
      _bindings.get_flight_by_index(idx, id, from, to, date, time, price, isFull);
      flights.add({
        'flightID':    int.tryParse(_readString(id)),
        'origin':      _readString(from),
        'destination': _readString(to),
        'date':        _readString(date),
        'time':        _readString(time),
        'price':       price.value,
        'isFull':      isFull.value == 1,
        'takenSeats':  <String>[],
      });
      calloc.free(id); calloc.free(from); calloc.free(to);
      calloc.free(date); calloc.free(time); calloc.free(price); calloc.free(isFull);
    }
    return flights;
  }

  // --- Tickets ---
  static int bookTicket(String passengerID, int flightID, String seat, bool isBusiness) {
    final p = passengerID.toNativeUtf8().cast<Char>();
    final s = seat.toNativeUtf8().cast<Char>();
    final result = _bindings.book_ticket(p, flightID, s, isBusiness ? 1 : 0);
    malloc.free(p); malloc.free(s);
    return result;
  }

  static bool cancelTicket(int ticketID) => _bindings.cancel_ticket(ticketID) == 1;

  static List<Map<String, dynamic>> getTicketsForPassenger(String passengerID) {
    final pid = passengerID.toNativeUtf8().cast<Char>();
    final count = _bindings.get_ticket_count_for_passenger(pid);
    final List<Map<String, dynamic>> tickets = [];

    for (int idx = 0; idx < count; idx++) {
      final ticketIDPtr = calloc<Int>(); 
      final flightIDPtr = calloc<Int>();
      final seatPtr = calloc<Char>(256); 
      final isBusinessPtr = calloc<Int>();
      final totalPricePtr = calloc<Double>();

      _bindings.get_ticket_by_index(pid, idx, ticketIDPtr, flightIDPtr, seatPtr, isBusinessPtr, totalPricePtr);

      final tID = ticketIDPtr.value;

      tickets.add({
        'ticketID':     tID,
        'flightID':     flightIDPtr.value,
        'seatNumber':   _readString(seatPtr),
        'isBusiness':   isBusinessPtr.value == 1,
        'totalPrice':   totalPricePtr.value,
        'totalWeight':  _bindings.get_ticket_total_weight(tID),
        'hasExtraFees': _bindings.ticket_has_extra_fees(tID) == 1,
        'extraFee':     _bindings.get_ticket_extra_fee(tID),
      });

      calloc.free(ticketIDPtr); 
      calloc.free(flightIDPtr);
      calloc.free(seatPtr); 
      calloc.free(isBusinessPtr); 
      calloc.free(totalPricePtr);
    }
    malloc.free(pid);
    return tickets;
  }

  // --- Luggage ---
  static bool addLuggage(int ticketID, double weight, bool isCabin) {
    return _bindings.add_luggage_to_ticket(ticketID, weight, isCabin ? 1 : 0) == 1;
  }

  static int getLuggageCount(int ticketID) => _bindings.get_luggage_count_for_ticket(ticketID);

  // --- Seats ---
  static List<String> getTakenSeats(int flightID, bool isBusiness){
    List<String> allSeats = SeatGenerator.generateSeats(isBusiness);
    List<String> available = getAvailableSeats(flightID, isBusiness);
    return allSeats.where((s)=>!available.contains(s)).toList();
  }

  // --- Users ---
  static List<Map<String, dynamic>> getAllUsers() {
    final count = _bindings.get_user_count();
    final List<Map<String, dynamic>> users = [];
    for (int idx = 0; idx < count; idx++) {
      final name = calloc<Char>(256); final id = calloc<Char>(256);
      _bindings.get_user_by_index(idx, name, id);
      users.add({'name': _readString(name), 'nationalID': _readString(id)});
      calloc.free(name); calloc.free(id);
    }
    return users;
  }

  // --- Passenger Info ---
  static Map<String, String> getPassengerInfo(String id) {
    final pid   = id.toNativeUtf8().cast<Char>();
    final name  = calloc<Char>(256);
    final email = calloc<Char>(256);
    final phone = calloc<Char>(256);
    _bindings.get_passenger_info(pid, name, email, phone);
    final result = {
      'name':  _readString(name),
      'email': _readString(email),
      'phone': _readString(phone),
    };
    malloc.free(pid);
    calloc.free(name); calloc.free(email); calloc.free(phone);
    return result;
  }

  // --- Edit Passenger ---
  static bool editPassengerPhone(String passengerID, String newPhone) {
    final p = passengerID.toNativeUtf8().cast<Char>();
    final n = newPhone.toNativeUtf8().cast<Char>();
    final result = _bindings.edit_passenger_phone(p, n);
    malloc.free(p); malloc.free(n);
    return result == 1;
  }

  static bool editPassengerEmail(String passengerID, String newEmail) {
    final p = passengerID.toNativeUtf8().cast<Char>();
    final n = newEmail.toNativeUtf8().cast<Char>();
    final result = _bindings.edit_passenger_email(p, n);
    malloc.free(p); malloc.free(n);
    return result == 1;
  }

  // --- Seats ---
  static List<String> getAvailableSeats(int flightID, bool isBusiness) {
    final count = _bindings.get_available_seat_count(flightID, isBusiness ? 1 : 0);
    final List<String> seats = [];
    
    for (int idx = 0; idx < count; idx++) {
      final seat = calloc<Char>(256);
      _bindings.get_seat_by_index(flightID, isBusiness ? 1 : 0, idx, seat);
      seats.add(_readString(seat));
      calloc.free(seat);
    }
    return seats;
  }

  // --- SaveFiles ---
  static void saveAllData() => _bindings.save_all_data();
  static void loadAllData() => _bindings.load_all_data();

  // --- SuperAdmin ---

  static bool addAdmin(String name, String id, String phone, String email) {
    final n = name.toNativeUtf8().cast<Char>();
    final i = id.toNativeUtf8().cast<Char>();
    final ph = phone.toNativeUtf8().cast<Char>();
    final e = email.toNativeUtf8().cast<Char>();
    final result = _bindings.add_admin(n, i, ph, e);
    malloc.free(n); malloc.free(i); malloc.free(ph); malloc.free(e);
    return result == 1;
  }

  static bool removeAdmin(String id) {
    final i = id.toNativeUtf8().cast<Char>();
    final result = _bindings.remove_admin(i);
    malloc.free(i);
    return result == 1;
  }

}