#include "leerAristas.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// Busca un punto por coordenadas exactas en la lista de puntos
Punto buscarPuntoConClase(double x, double y, const std::vector<Punto>& puntos) {
    for (const auto& p : puntos) {
        if (p.x == x && p.y == y) {
            return p; // Devuelve el punto con clase
        }
    }
    // Si no se encuentra, devuelve uno sin clase (esto puede reportarse también)
    return Punto(x, y, '?');
}

std::set<Arista> leerAristasDesdeCSV(const std::string& nombreArchivo, const std::vector<Punto>& puntos) {
    std::set<Arista> aristas;
    std::ifstream archivo(nombreArchivo);
    std::string linea;

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << nombreArchivo << std::endl;
        return aristas;
    }

    std::getline(archivo, linea); // Saltar encabezado

    while (std::getline(archivo, linea)) {
        if (std::count(linea.begin(), linea.end(), ';') != 3) continue;

        std::stringstream ss(linea);
        std::string valor;
        double x1, y1, x2, y2;

        std::getline(ss, valor, ';'); x1 = std::stod(valor);
        std::getline(ss, valor, ';'); y1 = std::stod(valor);
        std::getline(ss, valor, ';'); x2 = std::stod(valor);
        std::getline(ss, valor, ';'); y2 = std::stod(valor);

        if (x1 == x2 && y1 == y2) continue;

        Punto p1 = buscarPuntoConClase(x1, y1, puntos);
        Punto p2 = buscarPuntoConClase(x2, y2, puntos);

        aristas.insert(Arista(p1, p2));
    }

    return aristas;
}
