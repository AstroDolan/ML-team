#include "Triangulo.h"

Triangulo::Triangulo(const Punto& p1, const Punto& p2, const Punto& p3)
    : puntos{p1, p2, p3},
      aristas{Arista(p1, p2), Arista(p2, p3), Arista(p3, p1)},
      circuloCircunscritofigura(p1, p2, p3) {}

bool Triangulo::tieneArista(const Arista& a) const {
    for (int i = 0; i < 3; ++i) {
        if (aristas[i] == a) return true;
    }
    return false;
}

bool Triangulo::tienePunto(const Punto& p) const {
    for (int i = 0; i < 3; ++i) {
        if (puntos[i] == p) return true;
    }
    return false;
}

#include <array>
#include <algorithm>

bool Triangulo::operator<(const Triangulo& otro) const {
    std::array<Punto, 3> a = {puntos[0], puntos[1], puntos[2]};
    std::array<Punto, 3> b = {otro.puntos[0], otro.puntos[1], otro.puntos[2]};
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a < b;
}


#include <algorithm> // para std::count

bool Triangulo::operator==(const Triangulo& otro) const {
    int count = 0;
    for (const auto& p : puntos) {
        for (const auto& q : otro.puntos) {
            if (p == q) {
                count++;
                break;
            }
        }
    }
    return count == 3;
}


bool Triangulo::operator!=(const Triangulo& otro) const {
    return !(*this == otro);
}
