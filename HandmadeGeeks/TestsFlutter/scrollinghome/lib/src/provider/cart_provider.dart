// lib/src/provider/cart_provider.dart

import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:scrollinghome/src/model/cart_item.dart';
import 'package:scrollinghome/src/model/product_model.dart';

class CartNotifier extends StateNotifier<List<CartItem>> {
  CartNotifier() : super([]);

  void addToCart(Welcome product, {int quantity = 1}) {
    final index = state.indexWhere((item) => item.product.id == product.id);

    if (index != -1) {
      // Ya está en el carrito → aumentar cantidad
      state[index].quantity += quantity;
      state = [...state];
    } else {
      // Agregar nuevo
      state = [...state, CartItem(product: product, quantity: quantity)];
    }
  }

  void removeFromCart(int productId) {
    state = state.where((item) => item.product.id != productId).toList();
  }

  void clearCart() {
    state = [];
  }
}

final cartProvider = StateNotifierProvider<CartNotifier, List<CartItem>>(
  (ref) => CartNotifier(),
);
