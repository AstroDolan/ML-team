import 'package:scrollinghome/src/model/product_model.dart';

class ProductListState {
  final List<Welcome> products;
  final bool loading;
  final bool maxLoaded;

  ProductListState({
    required this.products,
    required this.loading,
    required this.maxLoaded,
  });

  ProductListState.initial({
    this.loading = false,
    this.maxLoaded = false,
  }): products = [];

  ProductListState copyWith({
    List<Welcome>? products,
    bool? loading,
    bool? maxLoaded,
  }) {
    return ProductListState(
      products: products ?? this.products,
      loading: loading ?? this.loading,
      maxLoaded: maxLoaded ?? this.maxLoaded,
    );
  }
}