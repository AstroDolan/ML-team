#pragma once
#include <vector>
#include <set>
#include "Punto.h"
#include "Arista.h"
#include "Triangulo.h"

class triangulacionDelaunay {
public:
    //vector puntos con los Punto, ordenado
    std::vector<Punto> puntos;
    //set triangulacion, para los Tringulo, elementos unicos
    std::set<Triangulo> triangulacion;


    //constructor que recibe un vector puntos con los Punto
    triangulacionDelaunay(const std::vector<Punto>& puntos);
    
    //metodo para triangular 
    void triangular();
    //metodos para obtener la triangulacion y la envolventeConvexa
    const std::set<Triangulo>& obtenerTriangulacion() const;
    // metodo para obtener el grafo de voronoi
    const std::set<Arista>& obtenerGrafoVoronoi() const;

private:
    //set para el grafo de voronoi 
    std::set<Arista> grafoVoronoi;
    //metodo para generar el supertriangulo
    Triangulo generarSuperTriangulo() const;
    //metodo para eliminar el supertriangulo y los triangulos con vertices al supertriangulo
    void eliminarTriangulosSuperTriangulo(const Triangulo& superTriangulo);
    //metodo para calcular el grafo de voronoi
    void calcularGrafoVoronoi();
};