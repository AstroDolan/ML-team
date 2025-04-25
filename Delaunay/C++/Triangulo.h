#pragma once
#include "Punto.h"
#include "Arista.h"
#include "circuloCircunscrito.h"

//clase triangulo
class Triangulo {
public:
	//array puntos con Punto y aristas con Arista
    Punto puntos[3];
    Arista aristas[3];
    //y circuloCircunscrito con circuloCircunscrito
    circuloCircunscrito circuloCircunscritofigura ;


    //constructor de un triangulo con 3 puntos Punto
    Triangulo(const Punto& punto1, const Punto& punto2, const Punto& punto3);
    bool tieneArista(const Arista& otraArista) const;
    bool tienePunto(const Punto& otroPunto) const;
    bool operator<(const Triangulo& otroTriangulo) const;
    bool operator==(const Triangulo& otroTriangulo) const;
    bool operator!=(const Triangulo& otroTriangulo) const;
}; 