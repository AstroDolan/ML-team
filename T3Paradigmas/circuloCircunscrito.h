#pragma once
#include "Punto.h"

class circuloCircunscrito {
public:
    Punto centro;
    float radio;

    circuloCircunscrito(const Punto& a, const Punto& b, const Punto& c);

    bool contienePunto(const Punto& p) const;

private:
    float calcularRadio(const Punto& a, const Punto& b, const Punto& c) const;
    Punto calcularCentro(const Punto& a, const Punto& b, const Punto& c) const;
};

