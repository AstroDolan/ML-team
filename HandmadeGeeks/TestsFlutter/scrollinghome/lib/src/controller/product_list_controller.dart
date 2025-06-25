import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:scrollinghome/src/repository/product_repository.dart';

final ProductCrontrollerProvider = Provider((ref){
  final repository = ref.watch(ProductRepositoryProvider);
  return ProductListController(repository: repository);
});

class ProductListController {
  final ProductRepository _repository;

  ProductListController({
    required ProductRepository repository,
  }) : _repository = repository;

  loadProducts() async {
    _repository.getProducts(skip: 0);
  }
}