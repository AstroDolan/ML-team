#include "triangulacionDelaunay.h"
#include <iostream>
#include <algorithm>
#include <map>

// constructor de la clase triangulacionDelaunay, recibe el vector puntos
triangulacionDelaunay::triangulacionDelaunay(const std::vector<Punto>& puntos) : puntos(puntos) {}

// meotodo para triangular, de la triangulacion de delaunay
void triangulacionDelaunay::triangular() {
    //comprobamos que el tamano del vector puntos sea mayor a 3, puntos necesario para construir un triangulo
    if (puntos.size() < 3) {
        std::cerr << "Error: Se necesitan al menos 3 puntos para la triangulación.\n";
        return;
    }
    //si hay mas de 3 puntos, generamos e instanciamos el super triangulo (aun sin implementar)
    Triangulo superTriangulo = generarSuperTriangulo();
    //metemos el supertriangulo en el set de triangulacion
    triangulacion.insert(superTriangulo);

    //iteramos sobre el vector puntos, para obtener los Punto
    for (int i = 0; i < puntos.size(); ++i) {
        const Punto& punto = puntos[i];
        std::set<Triangulo> triangulosMalos;
        std::set<Arista> poligono;

        // buscamos en el set triangulacion en cada triangulo, la circunferencia circunscrita que contenga el punto
        for (const auto& triangulo : triangulacion) {
            //si el triangulo contiene el punto, lo metemos en la lista triangulos malos, que eliminaremos al final
            if (triangulo.circuloCircunscritofigura.contienePunto(punto)) {
                triangulosMalos.insert(triangulo);
            }
        }

        // ahora buscamos las aristas de los triangulos que no sirven en triangulos malos
        for (const auto& triangulo : triangulosMalos) {
            for (const auto& arista : triangulo.aristas) {
                //contamos los triangulos malos con std::count_if
                //si el triangulo malo contiene la arista que estamos iterando, 
                if (std::count_if(triangulosMalos.begin(), triangulosMalos.end(),
                    //lambda, por referencia captura la arista [&arista] pudiendo usarla
                    //por lo que podemos encontrar si el triangulo t contiene la arista devolviendo un booleano
                    // la condicion es saber si es unica, solo una arista en el borde de un poligono que se forma con 
                    //los triangulos malos
                    [&arista](const Triangulo& t) { return t.tieneArista(arista); }) == 1) {
                    //si es unica y aparece en un solo triangulo malo, la anadimos a poligono
                    poligono.insert(arista);


                // y por eso mejor hubiese usado una lista vector, estaria indexado y utilizaria un for clasico con i en vez
                // de set, sin el count_if, being y end, aunque es mas rapido
                //podria cambiar el vector a set
                }
            }
        }

        // eliminamos los triangulos malos de la triangulacion
        for (const auto& triangulo : triangulosMalos) {
            triangulacion.erase(triangulo);
        }

        // recorremos poligono para crear nuevos triangulos, dandole los puntos con el punto
        // del primer for
        for (const auto& arista : poligono) {
            triangulacion.insert(Triangulo(arista.p1, arista.p2, punto));
        }
    }

    //despues de terminar los puntos eliminamos el supertriangulo

    eliminarTriangulosSuperTriangulo(superTriangulo);


    calcularGrafoVoronoi();
}

// metodo para la triangulacion resultante
const std::set<Triangulo>& triangulacionDelaunay::obtenerTriangulacion() const {
    return triangulacion;
}

// metodo para el super triangulo
Triangulo triangulacionDelaunay::generarSuperTriangulo() const {
    //inicialos porque si no, no compila jaja con el primero esta bien xdxdxd
    float minX = puntos[0].x, maxX = puntos[0].x;
    float minY = puntos[0].y, maxY = puntos[0].y;

    //recorremos el vector puntos y buscamos el minimo y maximo de x e y 
    for (const auto& punto : puntos) {
        minX = std::min(minX, punto.x);
        maxX = std::max(maxX, punto.x);
        minY = std::min(minY, punto.y);
        maxY = std::max(maxY, punto.y);
    }

    //calculamos el centro 
    float centroX = (minX + maxX) / 2.0f;
    float centroY = (minY + maxY) / 2.0f;
    // nos quedamos con la maxima distancia entre las diferencias de los maximos y minimos de x e y 
    float maxDistancia = std::max(maxX - minX, maxY - minY);

    //retornamos el super triangulo como clase Triangulo
    return Triangulo(
        Punto(centroX - 2 * maxDistancia, centroY - maxDistancia),
        Punto(centroX, centroY + 2 * maxDistancia),
        Punto(centroX + 2 * maxDistancia, centroY - maxDistancia)
    );
}

// metodo para eliminar los triangulos con puntos del super triangulo
void triangulacionDelaunay::eliminarTriangulosSuperTriangulo(const Triangulo& superTriangulo) {

    //recorremos el set de triangulacion con i 
    for (auto i = triangulacion.begin(); i != triangulacion.end();) {
        //comprobamos si el triangulo i tiene algun punto del supertriangulo
        if (i->tienePunto(superTriangulo.puntos[0]) ||
            i->tienePunto(superTriangulo.puntos[1]) ||
            i->tienePunto(superTriangulo.puntos[2])) {

            //si tiene alguno, lo eliminamos
            i = triangulacion.erase(i);
        } else {
            //+1 en la iteracion
            ++i;
        }
    }
}

// metodo para calcular el grafo de voronoi
void triangulacionDelaunay::calcularGrafoVoronoi() {
    // declaramos un map para los centros de las circunferencias circunscritas de cada triangulo en
    //centrosCirculosCircunscritos
    std::map<Triangulo, Punto> centrosCirculosCircunscritos;

    // for para triangulacion, para cada triangulo, para calcular los centros de los triangulo
    for (const auto& triangulo : triangulacion) {
        //calculamos el centro
        Punto centro = triangulo.circuloCircunscritofigura.centro; // Usamos el centro ya calculado
        //y lo guardamos en el map
        centrosCirculosCircunscritos[triangulo] = centro;
    }

    // for para triangulacion, para cada triangulo, para crear las aristas del grafo de voronoi
    for (const auto& triangulo : triangulacion) {
        // for para las aristas de cada triangulo
        for (const auto& arista : triangulo.aristas) {
            // buscamos los vecinos que compartan aristas
            for (const auto& vecino : triangulacion) {
                //como condicion, primero asegurarnos que no comparemos el triangulo consigo misma
                //y que el triangulo comparte la misma arista
                if (vecino != triangulo && vecino.tieneArista(arista)) {
                    // obtenemos los centros del los circulos cercanos
                    Punto centro1 = centrosCirculosCircunscritos[triangulo];
                    Punto centro2 = centrosCirculosCircunscritos[vecino];

                    // y creamos una arista con los centros 
                    Arista aristaVoronoi(centro1, centro2);
                    //anadimos la arista en el grafo de voronoi
                    grafoVoronoi.insert(aristaVoronoi); // Asumiendo que grafoVoronoi es un std::set<Arista>
                }
            }
        }
    }
}

//metodo para obtener el grafo de voronoi
const std::set<Arista>& triangulacionDelaunay::obtenerGrafoVoronoi() const {
    return grafoVoronoi;
}