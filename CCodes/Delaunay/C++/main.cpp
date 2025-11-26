//inspiracion https://github.com/SebastianBitsch/delauney-triangulation :))
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include "triangulacionDelaunay.h"
#include "envolventeConvexa.h"

// funcion para leer los csv
std::vector<Punto> leerPuntos(const std::string& nombreArchivo) {
    //declaramos el vector puntos con elementos Punto
    std::vector<Punto> puntos;
    //declaramos archivo para leer el csv
    std::ifstream archivo(nombreArchivo);
    //verificamos si el archivo se pudo abrir correctamente
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << nombreArchivo << std::endl;
        return puntos; 
    }
    //declaramos linea para leer cada linea
    std::string linea;


    //nos saltamos la primera linea, donde estan los titulos
    std::getline(archivo, linea);

    // creamos un ciclo mientras el archivo tenga lineas     
    while (std::getline(archivo, linea)) {
        //declaramos una cadena
        std::stringstream ss(linea);
        //declaramos un valor auxiliar
        std::string valor;
        float x, y;
        int z;

        //extraemos de el primer valor de ss hasta la coma
        std::getline(ss, valor, ',');
        x = std::stof(valor);
        //extraemos de el segundo valor de ss hasta la coma
        std::getline(ss, valor, ',');
        y = std::stof(valor);
        //extraemos de el tercer valor de ss hasta la coma
        std::getline(ss, valor, ',');
        z = std::stoi(valor);

        //creamos el objeto punto de la clase Punto
        Punto punto(x,y,z); 
        //y hacemos un push back al vector puntos
        puntos.push_back(punto);
    }

    return puntos;
}

// funcion para escribir el archivo de la triangulacion de Delaunay en un csv
// argumentos, el set triangulos, y el nombre del archivo
void escribirDelaunayCSV(const std::set<Triangulo>& triangulos, const std::string& nombreArchivo) {
    // declaramos archivo para abrir donde escribiremos
    std::ofstream archivo(nombreArchivo);
    //escribimos los titulos (tres coordenadas de cada triangulo)
    archivo << "x1,y1,x2,y2,x3,y3\n";
    //iteramos en triangulos, donde escribirremos las coordenadas de cada triangulo
    for (const auto& triangulo : triangulos) {
        archivo << triangulo.puntos[0].x << "," << triangulo.puntos[0].y << "," << triangulo.puntos[1].x << "," << triangulo.puntos[1].y << "," << triangulo.puntos[2].x << "," << triangulo.puntos[2].y << "\n";
    }
}

// funcion para escribir el archivo del grafo de voronoi en un csv
// argumentos, el set aristas, y el nombre del archivo
void escribirVoronoiCSV(const std::set<Arista>& aristas, const std::string& nombreArchivo) {
    // declaramos archivo para abrir donde escribiremos
    std::ofstream archivo(nombreArchivo);
    //escribimos los titulos (dos coordenadas de cada arista formada por los centros de los triangulos)
    archivo << "x1,y1,x2,y2\n";
    //iteramos en aristas, donde escribirremos las coordenadas de cada arista
    for (const auto& arista : aristas) {
        archivo << arista.p1.x << "," << arista.p1.y << "," << arista.p2.x << "," << arista.p2.y << "\n";
    }
}

// funcion para escribir el archivo de la envolvente convexa en un csv
// argumentos, el set aristas, y el nombre del archivo
void escribirEnvolventeCSV(const std::set<Arista>& aristas, const std::string& nombreArchivo) {
    // declaramos archivo para abrir donde escribiremos
    std::ofstream archivo(nombreArchivo);
    //escribimos los titulos (dos coordenadas de cada arista formando la envolvente convexa)
    archivo << "x1,y1,x2,y2\n";
    //iteramos en aristas, donde escribirremos las coordenadas de cada arista
    for (const auto& arista : aristas) {
        archivo << arista.p1.x << "," << arista.p1.y << "," << arista.p2.x << "," << arista.p2.y << "\n";
    }
}

int main() {
    std::string archivoCSV;
    std::cout<< "Ingrese el nombre del archivo CSV: ";
    std::cin >> archivoCSV;
    archivoCSV.append(".csv");

    // leemos los puntos del archivo puntos.csv
    std::vector<Punto> puntos = leerPuntos(archivoCSV);

    //verificamos si se leyeron bien los puntos (otro if como en la apertura del archivo, en este caso puede estar vacio (?))
    if (puntos.empty()) {
        std::cerr << "No se pudieron leer puntos del archivo. El programa se cerrará." << std::endl;
        return 1; //salimos del programa
    }

    // Crear una instancia de TriangulacionDelaunay y realizar la triangulación
    // instanciamos triangulacionDelaunay y aplicamos triangular
    triangulacionDelaunay triangulacion(puntos);
    triangulacion.triangular();

    // obtenemos la triangulacion y la escribimos
    const auto& triangulos = triangulacion.obtenerTriangulacion();
    escribirDelaunayCSV(triangulos, "delaunay.csv");

    //generamos un mensaje de exito para Delaunay
    std::cout << "Triangulación de Delaunay generado con éxito." << std::endl;

    // obtenemos el grafo de voronoi y la escribimos
    const auto& aristasVoronoi = triangulacion.obtenerGrafoVoronoi();
    escribirVoronoiCSV(aristasVoronoi, "voronoi.csv");

    //generamos un mensaje de exito para Voronoi
    std::cout << "Grafo de Voronoi generado con éxito." << std::endl;

    //instanciamos la envolvente convexa sobre las aristas del grafo de voronoi
    envolventeConvexa envolvente(aristasVoronoi);

    //calculamos la envolvente convexa
    envolvente.calcularEnvolventeConvexa();

    //obtenemos la envolvente convexa y la escribimos en un csv =))))
    const auto& aristasEnvolvente = envolvente.obtenerEnvolventeConvexa();
    escribirEnvolventeCSV(aristasEnvolvente, "envolvente_convexa.csv");

    //generamos un mensaje de exito para Voronoi
    std::cout << "Envolvente Convexa generado con éxito." << std::endl;


    return 0;
}