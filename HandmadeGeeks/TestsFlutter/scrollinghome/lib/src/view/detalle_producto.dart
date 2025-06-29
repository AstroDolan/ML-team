// lib/src/view/product_detail.dart

import 'package:flutter/material.dart';
import 'package:scrollinghome/src/model/product_model.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:scrollinghome/src/provider/cart_provider.dart';


class DetalleProducto extends ConsumerStatefulWidget {
  final Welcome producto;

  const DetalleProducto({super.key, required this.producto});

  @override
ConsumerState<DetalleProducto> createState() => _DetalleProductoState();
}

class _DetalleProductoState extends ConsumerState<DetalleProducto> {

  int cantidad = 1;

  void aumentarCantidad() {
    if (cantidad < widget.producto.stock) {
      setState(() {
        cantidad++;
      });
    }
  }

  void disminuirCantidad() {
    if (cantidad > 1) {
      setState(() {
        cantidad--;
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    final producto = widget.producto;

    return Scaffold(
      appBar: AppBar(title: Text(producto.title)),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Center(
              child: Image.network(
                producto.thumbnail,
                height: 180,
                fit: BoxFit.contain,
              ),
            ),
            const SizedBox(height: 16),
            const Text('Descripción:', style: TextStyle(fontWeight: FontWeight.bold)),
            Text(producto.description),
            const SizedBox(height: 16),
            Text('Precio: \$${producto.price}', style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
            const SizedBox(height: 16),
            Text('Stock disponible: ${producto.stock}'),
            const SizedBox(height: 16),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                IconButton(onPressed: disminuirCantidad, icon: const Icon(Icons.remove)),
                Text('$cantidad', style: const TextStyle(fontSize: 18)),
                IconButton(onPressed: aumentarCantidad, icon: const Icon(Icons.add)),
              ],
            ),
            const SizedBox(height: 16),
            Center(
              child: ElevatedButton(
                onPressed: () {
                  // Acción para agregar al carrito
                  ref.read(cartProvider.notifier).addToCart(widget.producto, quantity: cantidad);
                  ScaffoldMessenger.of(context).showSnackBar(
                    SnackBar(content: Text('Agregado $cantidad al carrito')),
                  );
                },
                child: const Text('Agregar al carrito'),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
