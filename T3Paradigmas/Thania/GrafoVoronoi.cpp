#include "GrafoVoronoi.h"
#include <fstream>

GrafoVoronoi::GrafoVoronoi(const std::set<Arista>& aristas, const std::vector<Punto>& puntos)
    : aristas(aristas), puntos(puntos) {}

char GrafoVoronoi::clasePunto(const Punto& p) const {
    for (const auto& pt : puntos) {
        if (pt == p) return pt.clase;
    }
    return '?'; // no encontrado
}

void GrafoVoronoi::generarPartition() {
    partition.clear();

    for (const auto& a : aristas) {
        char clase1 = clasePunto(a.p1);
        char clase2 = clasePunto(a.p2);

        if (clase1 != '?' && clase2 != '?' && clase1 != clase2) {
            partition.push_back(a); // Une un R y un A → frontera
        }
    }
}

void GrafoVoronoi::guardarPartitionCSV(const std::string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo);
    archivo << "x1,y1,x2,y2\n";

    for (const auto& a : partition) {
        archivo << a.p1.x << "," << a.p1.y << ","
                << a.p2.x << "," << a.p2.y << "\n";
    }
}
