#pragma once
#include "Punto.h"
#include "Arista.h"
#include "circuloCircunscrito.h"

class Triangulo {
public:
    Punto puntos[3];
    Arista aristas[3];
    circuloCircunscrito circuloCircunscritofigura;

    // Constructor
    Triangulo(const Punto& p1, const Punto& p2, const Punto& p3);

    // Métodos útiles
    bool tieneArista(const Arista& a) const;
    bool tienePunto(const Punto& p) const;

    // Comparadores
    bool operator<(const Triangulo& otro) const;
    bool operator==(const Triangulo& otro) const;
    bool operator!=(const Triangulo& otro) const;
};
