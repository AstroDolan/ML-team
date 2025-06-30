import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:scrollinghome/src/model/product_model.dart';

class FollowedNotifier extends StateNotifier<List<Welcome>> {
  FollowedNotifier() : super([]);

  void toggleFollow(Welcome product) {
    final exists = state.any((p) => p.id == product.id);
    if (exists) {
      state = state.where((p) => p.id != product.id).toList();
    } else {
      state = [...state, product];
    }
  }

  bool isFollowed(Welcome product) {
    return state.any((p) => p.id == product.id);
  }

  void removeFollow(int productId) {
    state = state.where((p) => p.id != productId).toList();
  }
}

final followedProvider = StateNotifierProvider<FollowedNotifier, List<Welcome>>((ref) {
  return FollowedNotifier();
});
