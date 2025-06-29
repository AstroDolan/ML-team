// lib/src/model/cart_item.dart

import 'package:scrollinghome/src/model/product_model.dart';

class CartItem {
  final Welcome product;
  int quantity;

  CartItem({required this.product, this.quantity = 1});
}
