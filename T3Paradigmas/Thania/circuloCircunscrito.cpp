#include "circuloCircunscrito.h"
#include <cmath>
#include <limits>

circuloCircunscrito::circuloCircunscrito(const Punto& a, const Punto& b, const Punto& c)
    : centro(0, 0), radio(0) {
    double det = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
    if (det == 0) {
        centro = a;
        radio = 0;
    } else {
        centro = calcularCentro(a, b, c);
        radio = calcularRadio(a, b, c);
    }
}

bool circuloCircunscrito::contienePunto(const Punto& p) const {
    return p.calcularDistancia(centro) < radio;
}

float circuloCircunscrito::calcularRadio(const Punto& a, const Punto& b, const Punto& c) const {
    float ab = a.calcularDistancia(b);
    float bc = b.calcularDistancia(c);
    float ac = c.calcularDistancia(a);
    float area2 = std::abs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    return (ab * bc * ac) / (2 * area2);
}

Punto circuloCircunscrito::calcularCentro(const Punto& a, const Punto& b, const Punto& c) const {
    float d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    float ux = ((a.x * a.x + a.y * a.y) * (b.y - c.y)
               + (b.x * b.x + b.y * b.y) * (c.y - a.y)
               + (c.x * c.x + c.y * c.y) * (a.y - b.y)) / d;
    float uy = ((a.x * a.x + a.y * a.y) * (c.x - b.x)
               + (b.x * b.x + b.y * b.y) * (a.x - c.x)
               + (c.x * c.x + c.y * c.y) * (b.x - a.x)) / d;
    return Punto(ux, uy);
}
