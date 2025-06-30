import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:scrollinghome/src/model/product_model.dart';
import 'package:scrollinghome/src/provider/followed_provider.dart';
import 'package:scrollinghome/src/view/detalle_producto.dart';

class ProductListTile extends ConsumerWidget {
  const ProductListTile({super.key, required this.product});
  final Welcome product;

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final isFollowed = ref.watch(followedProvider).any((p) => p.id == product.id);
    final size = MediaQuery.of(context).size;

    return GestureDetector(
      onTap: () {
        Navigator.push(
          context,
          MaterialPageRoute(
            builder: (_) => DetalleProducto(producto: product),
          ),
        );
      },
      child: Container(
        height: size.height,
        padding: const EdgeInsets.symmetric(horizontal: 30, vertical: 40),
        alignment: Alignment.center,
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Stack(
              alignment: Alignment.topRight,
              children: [
                Center(
                  child: Image.network(
                    product.thumbnail,
                    height: 250,
                    fit: BoxFit.contain,
                  ),
                ),
                Positioned(
                  top: 0,
                  right: 0,
                  child: GestureDetector(
                    onTap: () {
                      ref.read(followedProvider.notifier).toggleFollow(product);
                    },
                    child: Container(
                      decoration: BoxDecoration(
                        shape: BoxShape.circle,
                        color: isFollowed ? Colors.red : Colors.white,
                        border: Border.all(
                          color: isFollowed ? Colors.red : Colors.black,
                          width: 1.5,
                        ),
                      ),
                      padding: const EdgeInsets.all(6),
                      child: Icon(
                        isFollowed ? Icons.favorite : Icons.favorite_border,
                        color: isFollowed ? Colors.white : Colors.black,
                      ),
                    ),
                  ),
                ),
              ],
            ),
            const SizedBox(height: 20),
            Text(
              product.title,
              textAlign: TextAlign.center,
              style: const TextStyle(
                fontSize: 28,
                fontWeight: FontWeight.bold,
              ),
            ),
            const SizedBox(height: 10),
            Text(
              "\$${product.price}",
              style: const TextStyle(
                fontSize: 24,
                fontWeight: FontWeight.bold,
              ),
            ),
          ],
        ),
      ),
    );
  }
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
