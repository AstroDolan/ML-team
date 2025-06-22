#include "envolventeConvexa.h"
#include <algorithm>

// constructor
envolventeConvexa::envolventeConvexa(const std::set<Arista>& aristas) {
    extraerPuntosDeAristas(aristas);
}

//metodo para extraer puntos de las aristas y los almacenamos en un set
void envolventeConvexa::extraerPuntosDeAristas(const std::set<Arista>& aristas) {
    for (const auto& arista : aristas) {
        puntos.insert(arista.p1);
        puntos.insert(arista.p2);
    }
}


//metodo para determinar orientacion, 0 colineales, 1 horario y 2 antihorario
int envolventeConvexa::orientacion(const Punto& p, const Punto& q, const Punto& r) const {
    int productocruz = (r.x - q.x) * (q.y - p.y) - (r.y - q.y) * (q.x - p.x);
    if (productocruz == 0) 
        return 0;
    return (productocruz > 0) ? 1 : 2;
}

//calcular la envolvente convexa
void envolventeConvexa::calcularEnvolventeConvexa() {
    // nos aseguramos que el vector puntos tenga mas de 3 puntos
    if (puntos.size() < 3) 
        return;

    //pasamos de set a vector
    std::vector<Punto> puntosVector(puntos.begin(), puntos.end());

    Punto p0 = puntosVector[0];
    //iteramos sobre los puntos buscando el punto mas bajo sobre y o sobre y e x
    for (const auto& punto : puntosVector) {
        //si el punto es el mas bajo, sera la p0
        if (punto.y < p0.y || (punto.y == p0.y && punto.x < p0.x)) {
            p0 = punto;
        }
    }
    // ordenamos los puntos segun p0 y el angulo polar y usamos las orientacion
    //recorremos los puntos e iteramos en i y j, para cada i todos los j 
    //sirve un std::sort tambien pero hay que practicar los for clasicos y los vectores indexados
    for (int i = 0; i < puntosVector.size(); ++i) {
        for (int j = i + 1; j < puntosVector.size(); ++j) {
            Punto p1 = puntosVector[i];
            Punto p2 = puntosVector[j];
            //calculamos la orientacion
            int ori = orientacion(p0, p1, p2);
            //si la orientacion 0
            if (ori == 0) {
                //calculamos la distancias, si de p0 a p1 es menor que de p0 a p2
                if (p0.calcularDistancia(p1) > p0.calcularDistancia(p2)) {
                    //intercambiamos las posiciones en el vector
                    std::swap(puntosVector[i], puntosVector[j]);
                }
            //si la orientacion es distinta de 2, osea 1, hacemos el intercambio de posiciones,
            //y si es 2, lo dejamos como esta.
            } else if (ori != 2) {
                std::swap(puntosVector[i], puntosVector[j]);
            }
        }
    }

    // construimos la envolvente 
    std::vector<Punto> envolventeAux;
    //agregamos el punto p0
    envolventeAux.push_back(p0);
    //iteramos sobre los puntos
    for (int i = 0; i < puntosVector.size(); ++i) {
        // instanciamos el punto
        Punto punto = puntosVector[i];
        while (true) {
            //mientras el tamano de envolvente sea mayor a 1
            if (envolventeAux.size() > 1) {
                //instanciamos la orientacion de los dos ultimos puntos y el punto que estamos iterando 
                int ori = orientacion(envolventeAux[envolventeAux.size() - 2], envolventeAux.back(), punto);
                // mientras la orientacion no sea antihoraria
                if (ori != 2) {
                    //eliminamos el ultimo punto de la envolvente
                    envolventeAux.pop_back();
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        // si no, lo agregamos a la envolvente convexa
        envolventeAux.push_back(punto);
    }

    //por ultimo convertimos la envolvente de puntos a aristas
    for (size_t i = 0; i < envolventeAux.size(); ++i) {
        Punto p1 = envolventeAux[i];
        Punto p2 = envolventeAux[(i + 1) % envolventeAux.size()]; // Ciclar al primer punto
        envolvente.insert(Arista(p1, p2));
    }

}

// Método para obtener los puntos de la envolvente convexa
const std::set<Arista>& envolventeConvexa::obtenerEnvolventeConvexa() const {
    return envolvente;
}
