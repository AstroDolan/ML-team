#include <stdbool.h> // para usar el tipo bool, true, y false
#include <stdio.h>
#include <stdlib.h>

// Definir el tamaño de la cuadrícula
#define N 10

// Estructura para representar un par de coordenadas (x, y)
typedef struct {
    int first;
    int second;
} Pair;

// Función para comprobar si un punto está en el conjunto de visitados
bool isVisited(Pair visited[], int size, Pair pt) {
    for (int i = 0; i < size; i++) {
        if (visited[i].first == pt.first && visited[i].second == pt.second) {
            return true;
        }
    }
    return false;
}

// Función para verificar si un punto es válido
bool isValid(Pair visited[], int size, Pair pt) {
    return (pt.first >= 0) && (pt.first < N) &&
           (pt.second >= 0) && (pt.second < N) &&
           !isVisited(visited, size, pt);
}

int main() {
    // Array de puntos visitados
    Pair visited[100];
    int visitedSize = 0;

    // Ejemplo de punto a verificar
    Pair pt = {5, 5};

    // Verificar si el punto es válido
    if (isValid(visited, visitedSize, pt)) {
        printf("El punto es válido.\n");
    } else {
        printf("El punto no es válido.\n");
    }

    return 0;
}
