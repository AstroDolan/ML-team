#include "circuloCircunscrito.h"
#include <cmath>
#include <limits>

// constructor de la clase circuloCircunscrito
circuloCircunscrito::circuloCircunscrito(const Punto& puntoA, const Punto& puntoB, const Punto& puntoC) 
    : centro(0,0,0), radio(0) {
    // los puntos podrias ser colineales 
    if (puntoA.x * (puntoB.y - puntoC.y) + puntoB.x * (puntoC.y - puntoA.y) + puntoC.x * (puntoA.y - puntoB.y) == 0) {
        // los puntos son colineales si el determinante es 0
        // asignamos un centro sobre cualquier puento
        centro = puntoA;
        // y le asignamos un radio 0
        radio = 0;
    } else {
        //sino, calculamos un centro con calcularCentro
        centro = calcularCentro(puntoA, puntoB, puntoC);
        //y calculamos el radio con calcualarRadio
        radio = calcularRadio(puntoA, puntoB, puntoC);
    }
}

// verificar si el punto esta dentro del circulo, calculamos la distancia del punto al centro
// y la comparamos con el radio
bool circuloCircunscrito::contienePunto(const Punto& punto) const {
    return punto.calcularDistancia(centro) < radio;
}

// metodo para calcular el radio
float circuloCircunscrito::calcularRadio(const Punto& puntoA, const Punto& puntoB, const Punto& puntoC) const {
    //calculamos las distancias de las aristas
    float distanciaAB = puntoA.calcularDistancia(puntoB);
    float distanciaBC = puntoB.calcularDistancia(puntoC);
    float distanciaAC = puntoC.calcularDistancia(puntoA);

    //calculamos el determinante, el valor absoluto
    float determinante = std::abs(puntoA.x * (puntoB.y - puntoC.y) + puntoB.x * (puntoC.y - puntoA.y) + puntoC.x * (puntoA.y - puntoB.y));

    //radio
    return (distanciaAB * distanciaBC * distanciaAC) / (2 * determinante);
}

// metodo para calcular el centro de la circunferencia circunscrita
Punto circuloCircunscrito::calcularCentro(const Punto& puntoA, const Punto& puntoB, const Punto& puntoC) const {
    float determinante  = 2 * (puntoA.x * (puntoB.y - puntoC.y) + puntoB.x * (puntoC.y - puntoA.y) + puntoC.x * (puntoA.y - puntoB.y));
    float Ux = (std::pow(puntoA.x, 2) + std::pow(puntoA.y, 2)) * (puntoB.y - puntoC.y) + (std::pow(puntoB.x, 2) + std::pow(puntoB.y, 2)) * (puntoC.y - puntoA.y) + (std::pow(puntoC.x, 2) + std::pow(puntoC.y, 2)) * (puntoA.y - puntoB.y);
    float Uy = (std::pow(puntoA.x, 2) + std::pow(puntoA.y, 2)) * (puntoC.x - puntoB.x) + (std::pow(puntoB.x, 2) + std::pow(puntoB.y, 2)) * (puntoA.x - puntoC.x) + (std::pow(puntoC.x, 2) + std::pow(puntoC.y, 2)) * (puntoB.x - puntoA.x);
    return Punto(Ux / determinante, Uy / determinante);
}