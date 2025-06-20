#pragma once
#include "Punto.h"

class Arista {
public:
    Punto p1, p2;

    Arista();  // constructor por defecto
    Arista(const Punto& punto1, const Punto& punto2);

    bool operator==(const Arista& otra) const;
    bool operator<(const Arista& otra) const;
    bool contiene(const Punto& p) const;
};
