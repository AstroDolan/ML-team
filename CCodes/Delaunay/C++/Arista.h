#pragma once
#include "Punto.h"

//clase arista, conformada por dos puntos
class Arista {
public:
    //puntos para definir la arista
    Punto p1, p2;
    //constructor de la arista
    Arista(const Punto& p1, const Punto& p2);

    //sobrecarga en == para las aristas
    bool operator==(const Arista& otraArista) const;
    //sobrecarga en < para las aristas 
    bool operator<(const Arista& otraArista) const;
};