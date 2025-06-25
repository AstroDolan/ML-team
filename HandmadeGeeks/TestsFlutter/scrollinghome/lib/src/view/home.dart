import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:scrollinghome/src/controller/product_list_controller.dart';

class HomeView extends ConsumerWidget {
  const HomeView({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    ref.read(ProductCrontrollerProvider).loadProducts();
    return const Scaffold(
      body: Text("Home View"),  
    );
  }
}