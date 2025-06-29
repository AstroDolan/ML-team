// lib/src/view/cart_view.dart

import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:scrollinghome/src/provider/cart_provider.dart';
import 'detalle_producto.dart';

class CartView extends ConsumerWidget {
  const CartView({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final cartItems = ref.watch(cartProvider);

    if (cartItems.isEmpty) {
      return Scaffold(
        appBar: AppBar(title: Text("Carrito")),
        body: Center(child: Text("Tu carrito está vacío")),
      );
    }

    return Scaffold(
      appBar: AppBar(title: const Text("Carrito")),
      body: ListView.builder(
        itemCount: cartItems.length,
        itemBuilder: (_, index) {
          final item = cartItems[index];
          final product = item.product;

          return ListTile(
            leading: Image.network(product.thumbnail, width: 60),
            title: Text(product.title),
            subtitle: Text("\$${product.price} x ${item.quantity}"),
            trailing: IconButton(
              icon: const Icon(Icons.delete),
              onPressed: () {
                ref.read(cartProvider.notifier).removeFromCart(product.id);
              },
            ),
            onTap: () {
              Navigator.push(
                context,
                MaterialPageRoute(
                  builder: (_) => DetalleProducto(producto: product),
                ),
              );
            },
          );
        },
      ),
    );
  }
}
