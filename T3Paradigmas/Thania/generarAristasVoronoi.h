#pragma once
#include "Triangulo.h"
#include <set>

std::set<Arista> generarAristasVoronoi(const std::set<Triangulo>& triangulos);
void guardarAristasVoronoiCSV(const std::set<Arista>& aristas, const std::string& nombreArchivo);
