import 'dart:developer';
import 'package:http/http.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

final networkHandlerProvider = Provider((ref) => NetworkHandler());

class NetworkHandler{
  Future<Response?> getRequest({required String url}) async {
    final uri = Uri.parse(url);
    try{
      final response = await get(uri);
      return response;
    }catch(e){
      log("Failed to fetch data from $url: $e");
      return null;
    }
  }
}