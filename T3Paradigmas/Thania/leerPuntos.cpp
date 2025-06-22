#include "leerPuntos.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

std::vector<Punto> leerPuntos(const std::string& nombreArchivo) {
    std::vector<Punto> puntos;
    std::ifstream archivo(nombreArchivo);
    std::string linea;

    if (!archivo.is_open()) {
        std::cerr << "❌ Error al abrir el archivo: " << nombreArchivo << std::endl;
        return puntos;
    }

    std::getline(archivo, linea); // Saltar encabezado

    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        std::string xStr, yStr, claseStr;

        if (!std::getline(ss, xStr, ',')) continue;
        if (!std::getline(ss, yStr, ',')) continue;
        if (!std::getline(ss, claseStr, ',')) continue;

        try {
            double x = std::stod(xStr);
            double y = std::stod(yStr);
            char clase = toupper(claseStr[0]); // Asegura 'R' o 'A' en mayúscula

            if (clase != 'R' && clase != 'A') continue;

            puntos.emplace_back(x, y, clase);
        } catch (...) {
            // Si hay cualquier error de conversión, se salta la línea
            continue;
        }
    }

    return puntos;
}

