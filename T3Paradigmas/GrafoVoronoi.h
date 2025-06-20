#pragma once
#include <set>
#include <vector>
#include <string>
#include "Arista.h"
#include "Punto.h"

class GrafoVoronoi {
public:
    GrafoVoronoi(const std::set<Arista>& aristas, const std::vector<Punto>& puntos);

    void generarPartition();
    void guardarPartitionCSV(const std::string& nombreArchivo) const;

    std::vector<Arista> partition;

private:
    std::set<Arista> aristas;
    std::vector<Punto> puntos;

    char clasePunto(const Punto& p) const;
};
