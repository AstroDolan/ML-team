#include "leerAristas.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

std::set<Arista> leerAristasDesdeCSV(const std::string& nombreArchivo) {
    std::set<Arista> aristas;
    std::ifstream archivo(nombreArchivo);
    std::string linea;

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << nombreArchivo << std::endl;
        return aristas;
    }

    std::getline(archivo, linea); // Saltar encabezado

    while (std::getline(archivo, linea)) {
        // Asegurarse de que tenga 3 separadores ';'
        if (std::count(linea.begin(), linea.end(), ';') != 3) continue;

        std::stringstream ss(linea);
        std::string valor;
        double x1, y1, x2, y2;

        std::getline(ss, valor, ';'); x1 = std::stod(valor);
        std::getline(ss, valor, ';'); y1 = std::stod(valor);
        std::getline(ss, valor, ';'); x2 = std::stod(valor);
        std::getline(ss, valor, ';'); y2 = std::stod(valor);

        if (x1 == x2 && y1 == y2) continue;

        Punto p1(x1, y1);
        Punto p2(x2, y2);
        aristas.insert(Arista(p1, p2));
    }

    return aristas;
}
