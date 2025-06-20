#include "Arista.h" ,

Arista::Arista() : p1(), p2() {}

Arista::Arista(const Punto& punto1, const Punto& punto2)
    : p1(punto1), p2(punto2) {}

bool Arista::operator==(const Arista& otra) const {
    // Son iguales sin importar el orden de los puntos
    return (p1 == otra.p1 && p2 == otra.p2) || (p1 == otra.p2 && p2 == otra.p1);
}

bool Arista::operator<(const Arista& otra) const {
    // Orden lexicográfico (asegura que set<Arista> funcione bien)
    if (p1 != otra.p1) return p1 < otra.p1;
    return p2 < otra.p2;
}

bool Arista::contiene(const Punto& p) const {
    return p == p1 || p == p2;
}

