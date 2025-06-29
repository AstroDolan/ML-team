class Endpoints {
  const Endpoints._();

  static String productUrl({ required int skip, int limit = 20 }) => 
  "https://dummyjson.com/products?limit=$limit&skip=$skip";
}