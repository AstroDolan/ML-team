#pragma once

class Punto {
public:
    double x, y;
    char clase; // 'R' o 'A'

    Punto();  // constructor por defecto
    Punto(double x, double y, char clase = ' '); // constructor con clase
    double calcularDistancia(const Punto& otro) const;

    bool operator==(const Punto& otro) const;
    bool operator<(const Punto& otro) const;
    bool operator!=(const Punto& otro) const;
};
