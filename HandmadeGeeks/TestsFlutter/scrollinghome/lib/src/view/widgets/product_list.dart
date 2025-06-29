// lib/src/view/widgets/product_list.dart

import 'package:flutter/material.dart';
import 'package:scrollinghome/src/model/product_model.dart';
import 'package:scrollinghome/src/view/widgets/product_list_tile.dart';
import 'package:scrollinghome/src/service/product_service.dart';

class ProductList extends StatelessWidget {
  const ProductList({super.key});

  @override
  Widget build(BuildContext context) {
    return FutureBuilder<List<Welcome>>(
      future: ProductService.fetchProducts(),
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return const Center(child: CircularProgressIndicator());
        }

        if (snapshot.hasError) {
          return Center(child: Text("Error: ${snapshot.error}"));
        }

        final products = snapshot.data!;

        return PageView.builder(
          scrollDirection: Axis.vertical,
          itemCount: products.length,
          itemBuilder: (context, index) {
            return ProductListTile(product: products[index]);
          },
        );
      },
    );
  }
}
