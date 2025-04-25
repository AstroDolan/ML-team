Trabajo II: Programacion Orientada a Objetos
======================================================

Este proyecto es parte del trabajo de programación orientada a objetos para la asignatura Paradigmas de Programación de la carrera Licenciatura en Ciencias de la Computación de la Universidad de Santiago de Chile. El proyecto implementa la triangulación de Delaunay y el grafo de Voronoi para un conjunto de puntos en un plano 2D, incluyendo el cálculo de la envolvente convexa.

Objetivos del Proyecto:
-----------------------
1. Definir clases para contener puntos a clasificar en el plano (2D).
2. Implementar un algoritmo para crear una triangulación de Delaunay a partir de un conjunto arbitrario de puntos en el plano.
3. Obtener el grafo dual de Delaunay, conocido como el grafo de Voronoi.
4. Calcular la envolvente convexa del conjunto de puntos.

Requisitos del Sistema:
-----------------------
- Compilador de C++ que soporte C++11 o superior
- Sistema operativo Linux (probado en este entorno)

Archivos del Proyecto:
----------------------
- main.cpp: Archivo principal que contiene la función main()
- Punto.h y Punto.cpp: Definición e implementación de la clase Punto
- Arista.h y Arista.cpp: Definición e implementación de la clase Arista
- Triangulo.h y Triangulo.cpp: Definición e implementación de la clase Triangulo
- circuloCircunscrito.h y circuloCircunscrito.cpp: Definición e implementación de la clase circuloCircunscrito
- triangulacionDelaunay.h y triangulacionDelaunay.cpp: Definición e implementación de la clase triangulacionDelaunay
- envolventeConvexa.h y envolventeConvexa.cpp: Definición e implementación de la clase envolventeConvexa

Cómo Compilar y Ejecutar:
-------------------------
1. Compile el proyecto utilizando el siguiente comando en la terminal:
   g++ -std=c++11 main.cpp Punto.cpp Arista.cpp circuloCircunscrito.cpp Triangulo.cpp triangulacionDelaunay.cpp envolventeConvexa.cpp -o Trabajo2

   Nota: Alternativamente, puede usar el comando `g++ -std=c++11 *.cpp -o Trabajo2` para compilar todos los archivos .cpp en el directorio actual.

2. Ejecute el programa:
   ./Trabajo2

3. Cuando se le solicite, ingrese el nombre del archivo CSV de los puntos de entrada (sin la extensión .csv):
   puntosPrueba

Archivos de Salida:
-------------------
El programa generará los siguientes archivos de salida:
- delaunay.csv: Contiene los tres puntos de cada triángulo formado en la triangulación de Delaunay
- voronoi.csv: Contiene las aristas del grafo de Voronoi
- envolvente_convexa.csv: Contiene las aristas de la envolvente convexa

Notas:
------
- El programa puede manejar valores flotantes para las coordenadas de los puntos.
- Aunque el archivo de entrada incluye una columna z, estos valores no se utilizan en los cálculos principales.

Autor:
------
Nícolas Ilich Díaz González

Fecha de Entrega:
-----------------
19-08-2024
