#include "Triangulo.h"
#include "Arista.h"
#include <set>
#include <vector>
#include <fstream>

// Devuelve aristas del Voronoi conectando los centros de triángulos adyacentes
std::set<Arista> generarAristasVoronoi(const std::set<Triangulo>& triangulos) {
    std::set<Arista> aristasVoronoi;

    for (auto it1 = triangulos.begin(); it1 != triangulos.end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != triangulos.end(); ++it2) {
            // Si los triángulos comparten una arista, conectar sus centros
            for (int i = 0; i < 3; ++i) {
                Arista a = it1->aristas[i];
                if (it2->tieneArista(a)) {
                    Punto c1 = it1->circuloCircunscritofigura.centro;
                    Punto c2 = it2->circuloCircunscritofigura.centro;
                    aristasVoronoi.insert(Arista(c1, c2));
                }
            }
        }
    }

    return aristasVoronoi;
}

// Guarda las aristas en CSV para visualización
void guardarAristasVoronoiCSV(const std::set<Arista>& aristas, const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo);
    archivo << "x1;y1;x2;y2\n";
    for (const auto& a : aristas) {
        archivo << a.p1.x << ";" << a.p1.y << ";"
                << a.p2.x << ";" << a.p2.y << "\n";
    }
}
