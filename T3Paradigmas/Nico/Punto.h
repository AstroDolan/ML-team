#pragma once

class Punto {
public:
    //coordenadas (x,y) y z 
    float x, y;
    int z;

    Punto();
    //constructor para la clase punto
    Punto(float x, float y, int z = 0);
    //metodo para distancia entre dos puntos
    float calcularDistancia(const Punto& otroPunto) const;
    //sobrecarga en == para puntos
    bool operator==(const Punto& otroPunto) const;
    //sobrecarga en < para puntos
    bool operator<(const Punto& otroPunto) const;
    //sobrecarga en != para puntos
    bool operator!=(const Punto& otroPunto) const;
};