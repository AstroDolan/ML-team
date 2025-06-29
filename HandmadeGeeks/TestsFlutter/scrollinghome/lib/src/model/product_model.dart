import 'dart:convert';

Welcome welcomeFromJson(String str) => Welcome.fromJson(json.decode(str));

String welcomeToJson(Welcome data) => json.encode(data.toJson());

class Welcome {
    final int id;
    final String title;
    final String description;
    final String category;
    final double price;
    final double discountPercentage;
    final double rating;
    final int stock;
    final List<String> tags;
    final String brand;
    final String sku;
    final int weight;
    final Dimensions dimensions;
    final String warrantyInformation;
    final String shippingInformation;
    final String availabilityStatus;
    final List<Review> reviews;
    final String returnPolicy;
    final int minimumOrderQuantity;
    final Meta meta;
    final List<String> images;
    final String thumbnail;

    Welcome({
        required this.id,
        required this.title,
        required this.description,
        required this.category,
        required this.price,
        required this.discountPercentage,
        required this.rating,
        required this.stock,
        required this.tags,
        required this.brand,
        required this.sku,
        required this.weight,
        required this.dimensions,
        required this.warrantyInformation,
        required this.shippingInformation,
        required this.availabilityStatus,
        required this.reviews,
        required this.returnPolicy,
        required this.minimumOrderQuantity,
        required this.meta,
        required this.images,
        required this.thumbnail,
    });

factory Welcome.fromJson(Map<String, dynamic> json) => Welcome(
  id: json["id"] ?? 0,
  title: json["title"] ?? "Sin título",
  description: json["description"] ?? "Sin descripción",
  category: json["category"] ?? "N/A",
  price: (json["price"] ?? 0).toDouble(),
  discountPercentage: (json["discountPercentage"] ?? 0).toDouble(),
  rating: (json["rating"] ?? 0).toDouble(),
  stock: json["stock"] ?? 0,
  tags: List<String>.from(json["tags"] ?? []),
  brand: json["brand"] ?? "Marca desconocida",
  sku: json["sku"] ?? "SKU desconocido",
  weight: json["weight"] ?? 0,
  dimensions: json["dimensions"] != null
      ? Dimensions.fromJson(json["dimensions"])
      : Dimensions(width: 0, height: 0, depth: 0),
  warrantyInformation: json["warrantyInformation"] ?? "",
  shippingInformation: json["shippingInformation"] ?? "",
  availabilityStatus: json["availabilityStatus"] ?? "Desconocido",
  reviews: json["reviews"] != null
      ? List<Review>.from(json["reviews"].map((x) => Review.fromJson(x)))
      : [],
  returnPolicy: json["returnPolicy"] ?? "",
  minimumOrderQuantity: json["minimumOrderQuantity"] ?? 1,
  meta: json["meta"] != null
      ? Meta.fromJson(json["meta"])
      : Meta(
          createdAt: DateTime.now(),
          updatedAt: DateTime.now(),
          barcode: "",
          qrCode: "",
        ),
  images: List<String>.from(json["images"] ?? []),
  thumbnail: json["thumbnail"] ?? "https://via.placeholder.com/300",
);
    Map<String, dynamic> toJson() => {
        "id": id,
        "title": title,
        "description": description,
        "category": category,
        "price": price,
        "discountPercentage": discountPercentage,
        "rating": rating,
        "stock": stock,
        "tags": List<dynamic>.from(tags.map((x) => x)),
        "brand": brand,
        "sku": sku,
        "weight": weight,
        "dimensions": dimensions.toJson(),
        "warrantyInformation": warrantyInformation,
        "shippingInformation": shippingInformation,
        "availabilityStatus": availabilityStatus,
        "reviews": List<dynamic>.from(reviews.map((x) => x.toJson())),
        "returnPolicy": returnPolicy,
        "minimumOrderQuantity": minimumOrderQuantity,
        "meta": meta.toJson(),
        "images": List<dynamic>.from(images.map((x) => x)),
        "thumbnail": thumbnail,
    };
}

class Dimensions {
    final double width;
    final double height;
    final double depth;

    Dimensions({
        required this.width,
        required this.height,
        required this.depth,
    });

    factory Dimensions.fromJson(Map<String, dynamic> json) => Dimensions(
        width: json["width"]?.toDouble(),
        height: json["height"]?.toDouble(),
        depth: json["depth"]?.toDouble(),
    );

    Map<String, dynamic> toJson() => {
        "width": width,
        "height": height,
        "depth": depth,
    };
}

class Meta {
    final DateTime createdAt;
    final DateTime updatedAt;
    final String barcode;
    final String qrCode;

    Meta({
        required this.createdAt,
        required this.updatedAt,
        required this.barcode,
        required this.qrCode,
    });

    factory Meta.fromJson(Map<String, dynamic> json) => Meta(
        createdAt: DateTime.parse(json["createdAt"]),
        updatedAt: DateTime.parse(json["updatedAt"]),
        barcode: json["barcode"],
        qrCode: json["qrCode"],
    );

    Map<String, dynamic> toJson() => {
        "createdAt": createdAt.toIso8601String(),
        "updatedAt": updatedAt.toIso8601String(),
        "barcode": barcode,
        "qrCode": qrCode,
    };
}

class Review {
    final int rating;
    final String comment;
    final DateTime date;
    final String reviewerName;
    final String reviewerEmail;

    Review({
        required this.rating,
        required this.comment,
        required this.date,
        required this.reviewerName,
        required this.reviewerEmail,
    });

    factory Review.fromJson(Map<String, dynamic> json) => Review(
        rating: json["rating"],
        comment: json["comment"],
        date: DateTime.parse(json["date"]),
        reviewerName: json["reviewerName"],
        reviewerEmail: json["reviewerEmail"],
    );

    Map<String, dynamic> toJson() => {
        "rating": rating,
        "comment": comment,
        "date": date.toIso8601String(),
        "reviewerName": reviewerName,
        "reviewerEmail": reviewerEmail,
    };
}
