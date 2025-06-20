#include <iostream>              // Para std::cout
#include "leerAristas.h"        // Para leerAristasDesdeCSV
#include "leerPuntos.h"         // Para leerPuntos (Objetivo 3)
#include "GrafoVoronoi.h"       // Si ya estás armando el grafo



int main() {
    std::set<Arista> aristas = leerAristasDesdeCSV("edges-delaunay.csv");
    std::vector<Punto> puntos = leerPuntos("puntosPrueba.csv");

    GrafoVoronoi grafo(aristas, puntos);
    grafo.generarPartition();
    grafo.guardarPartitionCSV("partition.csv");

    std::cout << "partition.csv generado con " << grafo.partition.size()
              << " aristas que dividen regiones R y A.\n";

    return 0;
}
