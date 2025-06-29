// lib/src/service/product_service.dart

import 'package:http/http.dart' as http;
import 'dart:convert';

import '../model/product_model.dart';

class ProductService {
  static Future<List<Welcome>> fetchProducts({int skip = 0, int limit = 20}) async {
    final url = Uri.parse("https://dummyjson.com/products?limit=$limit&skip=$skip");
    final response = await http.get(url);

    if (response.statusCode == 200) {
      final data = json.decode(response.body);
      final List<dynamic> products = data['products'];
      return products.map((item) => Welcome.fromJson(item)).toList();
    } else {
      throw Exception('Failed to load products');
    }
  }
}
