#include <iostream>              // Para std::cout
#include "leerAristas.h"        // Para leerAristasDesdeCSV
#include "leerPuntos.h"         // Para leerPuntos (Objetivo 3)
#include "GrafoVoronoi.h"       // Si ya est�s armando el grafo
#include "generarAristasVoronoi.h" // Para generar aristas Voronoi
#include "generarTriangulosDesdeAristas.h" // Para generar triángulos desde aristas



int main() {
    std::vector<Punto> puntos = leerPuntos("puntosPrueba.csv");
    std::cout << "[INFO] Se leyeron " << puntos.size() << " puntos.\n";

    std::set<Arista> aristas = leerAristasDesdeCSV("edges-delaunay.csv", puntos);
    std::cout << "[INFO] Se leyeron " << aristas.size() << " aristas.\n";

    // Generar triángulos desde aristas
    std::set<Triangulo> triangulos = generarTriangulosDesdeAristas(aristas);
    std::cout << "[INFO] Se reconstruyeron " << triangulos.size() << " triángulos.\n";

    // Generar Voronoi desde triángulos
    std::set<Arista> voronoi = generarAristasVoronoi(triangulos);
    std::cout << "[INFO] Se generaron " << voronoi.size() << " aristas Voronoi.\n";
    guardarAristasVoronoiCSV(voronoi, "voronoi.csv");

    // Ahora sí, puedes usar GrafoVoronoi si deseas detectar la partición R/A
    GrafoVoronoi grafo(voronoi, puntos);
    grafo.generarPartition();
    std::cout << "[INFO] Fronteras detectadas: " << grafo.partition.size() << "\n";
    grafo.guardarPartitionCSV("partition.csv");

    std::cout << "Voronoi generado con " << voronoi.size() << " aristas.\n";
    std::cout << "Partition generado con " << grafo.partition.size() << " aristas frontera.\n";
    return 0;
}
