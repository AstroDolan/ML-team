#include "Triangulo.h"
#include <algorithm>

// constructor para la clase triangulo
Triangulo::Triangulo(const Punto& punto1, const Punto& punto2, const Punto& punto3)
    //array con los puntos, las aristas con los puntos y el circuloCircunscrito
    : puntos{punto1, punto2, punto3}, aristas{Arista(punto1, punto2), Arista(punto2, punto3), Arista(punto3, punto1)}, circuloCircunscritofigura(punto1, punto2, punto3) {}

// metodo para buscar una arista
bool Triangulo::tieneArista(const Arista& otraArista) const {
    // ciclo for para el array de las aristas
    for (int i = 0; i < 3; ++i) {
        // comparamos la arista actual con la arista buscada
        if (aristas[i] == otraArista) {
            return true; // sii encontramos la arista, devolvemos true
        }
    }
    return false; //si no encontramos la arista, devolvemos false
}

// metodo para buscar un punto especifico en un triangulo
bool Triangulo::tienePunto(const Punto& otroPunto) const {
    // ciclo for para el array de los puntos 
    for (int i = 0; i < 3; ++i) {
        // comparamos el punto actual con el punto buscado
        if (puntos[i] == otroPunto) {
            return true; // si encontramos el punto, devolvemos true
        }
    }
    return false; // si no encontramos el punto, devolvemos false
}
// sobrecarga de < para el triangulo y ordenar los triangulos
bool Triangulo::operator < (const Triangulo& otroTriangulo) const {
    //ciclo for para ordenar los triangulos, con los puntos distintos
    for (int i = 0; i < 3; ++i) {
        if (puntos[i] != otroTriangulo.puntos[i]) {
            return puntos[i] < otroTriangulo.puntos[i];
        }
    }
    return false;
}

bool Triangulo::operator == (const Triangulo& otroTriangulo) const {
        return (puntos[0] == otroTriangulo.puntos[0] &&
                puntos[1] == otroTriangulo.puntos[1] &&
                puntos[2] == otroTriangulo.puntos[2]);

}

bool Triangulo::operator!=(const Triangulo& otroTriangulo) const {
    return !(*this == otroTriangulo);
}