#include "Arista.h"

// constructor para la arista
Arista::Arista(const Punto& p1, const Punto& p2) : p1(p1), p2(p2) {}

// sobrecarga en == para las aristas, comparando puntos
bool Arista::operator==(const Arista& otraArista) const {
    return (p1 == otraArista.p1 && p2 == otraArista.p2) || (p1 == otraArista.p2 && p2 == otraArista.p1);
}

// sobrecarga en < para las aristas, para ordenarlas
bool Arista::operator<(const Arista& otraArista) const {
    if (p1 != otraArista.p1) {
        return p1 < otraArista.p1;
    } else {
        return p2 < otraArista.p2;
    }
}