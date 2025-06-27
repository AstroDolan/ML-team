import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:scrollinghome/src/controller/product_list_controller.dart';
import 'package:scrollinghome/src/view/widgets/product_list.dart';
class HomeView extends ConsumerWidget {
  const HomeView({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Productos"),
      ),
      body: const ProductList(),  
    );
  }
}