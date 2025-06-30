import 'dart:convert';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:http/http.dart' as http;
import 'package:scrollinghome/src/model/product_model.dart';

final productsProvider = FutureProvider<List<Welcome>>((ref) async {
  const int skip = 0;
  const int limit = 20;

  final response = await http.get(Uri.parse(
    "https://dummyjson.com/products?limit=$limit&skip=$skip",
  ));

  if (response.statusCode == 200) {
    final Map<String, dynamic> jsonResponse = json.decode(response.body);
    final List<dynamic> productsJson = jsonResponse["products"];
    return productsJson.map((json) => Welcome.fromJson(json)).toList();
  } else {
    throw Exception('Error al cargar productos');
  }
});
