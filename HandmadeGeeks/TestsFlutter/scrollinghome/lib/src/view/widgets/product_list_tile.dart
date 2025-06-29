// lib/src/view/widgets/product_list_tile.dart

import 'package:flutter/material.dart';
import 'package:scrollinghome/src/model/product_model.dart';
import 'package:scrollinghome/src/view/detalle_producto.dart'; // Asegúrate de importar correctamente

class ProductListTile extends StatefulWidget {
  const ProductListTile({super.key, required this.product});

  final Welcome product;

  @override
  State<ProductListTile> createState() => _ProductListTileState();
}

class _ProductListTileState extends State<ProductListTile> {
  bool isFavorite = false;

  void toggleFavorite() {
    setState(() {
      isFavorite = !isFavorite;
      // Aquí puedes guardar/eliminar el producto de una lista real
    });
  }

  @override
  Widget build(BuildContext context) {
    final size = MediaQuery.of(context).size;

    return GestureDetector(
      onTap: () {
        Navigator.push(
          context,
          MaterialPageRoute(
            builder: (_) => DetalleProducto(producto: widget.product),
          ),
        );
      },
      child: SingleChildScrollView(
        child: Container(
          height: size.height,
          padding: const EdgeInsets.all(30),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Stack(
                children: [
                  Image.network(
                    widget.product.thumbnail,
                    height: 250,
                    fit: BoxFit.contain,
                  ),
                  Positioned(
                    top: 8,
                    right: 8,
                    child: GestureDetector(
                      onTap: () {
                        toggleFavorite();
                      },
                      child: Container(
                        decoration: BoxDecoration(
                          shape: BoxShape.circle,
                          color: isFavorite ? Colors.red : Colors.white,
                          border: Border.all(
                            color: isFavorite ? Colors.red : Colors.black,
                            width: 1.5,
                          ),
                        ),
                        padding: const EdgeInsets.all(6),
                        child: Icon(
                          isFavorite ? Icons.favorite : Icons.favorite_border,
                          color: isFavorite ? Colors.white : Colors.black,
                          size: 24,
                        ),
                      ),
                    ),
                  ),
                ],
              ),
              const SizedBox(height: 20),
              Text(
                widget.product.title,
                style: const TextStyle(fontSize: 28, fontWeight: FontWeight.bold),
                textAlign: TextAlign.center,
              ),
              const SizedBox(height: 10),
              Text(
                "\$${widget.product.price}",
                style: const TextStyle(fontSize: 24, fontWeight: FontWeight.bold),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
