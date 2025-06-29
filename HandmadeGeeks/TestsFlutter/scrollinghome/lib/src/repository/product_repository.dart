import 'dart:convert';
import 'dart:developer';
import 'package:scrollinghome/src/network/network.dart';
import 'package:scrollinghome/src/res/endpoints.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:scrollinghome/src/model/product_model.dart';


final productRepositoryProvider = Provider((ref) {
  final networkHandler = ref.watch(networkHandlerProvider);
  return ProductRepository(networkHandler: networkHandler);
});

class ProductRepository {
  final NetworkHandler _networkHandler;

  ProductRepository({
    required NetworkHandler networkHandler,
  }): _networkHandler = networkHandler;

  Future<List<Welcome>?> getProducts({ required int skip, int limit = 10 }) async {
    log("Products Requested");

    final url = Endpoints.productUrl(skip: skip, limit: limit);
    final response = await _networkHandler.getRequest(url: url);
    if(response!=null){
      log(response.body);
      final jsonData = jsonDecode(response.body);
      final List productsJson = jsonData['products']; 
      final products = productsJson.map((json) => Welcome.fromJson(json)).toList();
      return products;
    } else {
      log("Error fetching products");
      return null;
    }
  }
}