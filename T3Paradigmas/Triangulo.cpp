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

bool Triangulo::operator<(const Triangulo& otro) const {
    for (int i = 0; i < 3; ++i) {
        if (puntos[i] != otro.puntos[i])
            return puntos[i] < otro.puntos[i];
    }
    return false;
}

bool Triangulo::operator==(const Triangulo& otro) const {
    return puntos[0] == otro.puntos[0] &&
           puntos[1] == otro.puntos[1] &&
           puntos[2] == otro.puntos[2];
}

bool Triangulo::operator!=(const Triangulo& otro) const {
    return !(*this == otro);
}
