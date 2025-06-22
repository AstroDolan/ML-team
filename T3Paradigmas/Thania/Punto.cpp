#include "Punto.h"
#include <cmath>

Punto::Punto() : x(0), y(0), clase(' ') {}

Punto::Punto(double x, double y, char clase) : x(x), y(y), clase(clase) {}

double Punto::calcularDistancia(const Punto& otro) const {
    return std::sqrt(std::pow(x - otro.x, 2) + std::pow(y - otro.y, 2));
}

bool Punto::operator==(const Punto& otro) const {
    const double epsilon = 1e-6;
    return std::abs(x - otro.x) < epsilon && std::abs(y - otro.y) < epsilon;
}


bool Punto::operator<(const Punto& otro) const {
    return (x != otro.x) ? (x < otro.x) : (y < otro.y);
}

bool Punto::operator!=(const Punto& otro) const {
    return !(*this == otro);
}
