#include "Punto.h"
#include <cmath>

//constructor por defecto, problemas con el map
Punto::Punto() : x(0), y(0), z(0) {}

//constructor
Punto::Punto(float x, float y, int z) : x(x), y(y), z(z) {}

//metodo para la distancia entre dos puntos, distancia euclidiana
float Punto::calcularDistancia(const Punto& otroPunto) const { 
    return std::sqrt(std::pow(x - otroPunto.x, 2) + std::pow(y - otroPunto.y, 2));
}

//sobrecarga en == para puntos
bool Punto::operator==(const Punto& otroPunto) const {
    return x == otroPunto.x && y == otroPunto.y;
}

//sobrecarga en < para puntos
bool Punto::operator<(const Punto& otroPunto) const {
    if (x != otroPunto.x) {
        return x < otroPunto.x;
    } else {
        return y < otroPunto.y;
    }
}

bool Punto::operator!=(const Punto& otroPunto) const {
    return !(*this == otroPunto);
}