#pragma once
#include "Punto.h"
//circulo circunscrito entre tres puntos
class circuloCircunscrito {
public:
    // variables centro y radio
    Punto centro;
    float radio;

    //constructor con tres puntos
    circuloCircunscrito(const Punto& puntoA, const Punto& puntoB, const Punto& puntoC);
    //verificar si un punto esta dentro o justo (el det tendria que ser = 0 pero no se 
    //si me producira problemas con la coma flotante) o fuera del circulo
    bool contienePunto(const Punto& punto) const;

private:
    //calcular radio del circulo circunscrito
    float calcularRadio(const Punto& puntoA, const Punto& puntoB, const Punto& puntoC) const;
    //calcular el centro del circulo circunscrito (para voronoi)
    Punto calcularCentro(const Punto& puntoA, const Punto& puntoB, const Punto& puntoC) const;
};