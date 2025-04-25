 // Algoritmo de graham, o intento https://es.wikipedia.org/wiki/M%C3%A9todo_de_Graham

#pragma once
#include <set>
#include <vector>
#include "Arista.h"
#include "Punto.h"

class envolventeConvexa {
public:
	//construcor de la envolvente convexa, recibe un set de Arista del grafo de Voronoi
	envolventeConvexa(const std::set<Arista>& aristas);


	// metodo para calcular la envolvente convexa
    void calcularEnvolventeConvexa();

    // metodo para obtener la envolvente convexa
    const std::set<Arista>& obtenerEnvolventeConvexa() const;

private:
    std::set<Punto> puntos; //puntos del grafo de voronoi, que si estan en set REVISADO
    std::set<Arista> envolvente; // aristas de la envolvente, en set

    // para determinar la orientacion de los tres puntos
    int orientacion(const Punto& p, const Punto& q, const Punto& r) const;

    // metodo para extraer puntos de aristas
    void extraerPuntosDeAristas(const std::set<Arista>& aristas);
};