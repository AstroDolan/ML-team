#pragma once
#include <set>
#include <vector>
#include <string>
#include "Arista.h"
#include "Punto.h"

std::set<Arista> leerAristasDesdeCSV(const std::string& nombreArchivo, const std::vector<Punto>& puntos);
