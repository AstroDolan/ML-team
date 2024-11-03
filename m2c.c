#include <stdbool.h> // para usar el tipo bool, true, y false
#include <stdio.h>
#include <stdlib.h>

// Definir el tamaño de la cuadrícula
#define N 9

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

//REVISAR SI AQUI ES NECESARIO USAR *SIZE EN VEZ DE SIZE
void addVisited(Pair visited[], int *size, Pair pt){
    visited[*size] = pt;
    (*size)++;
}

void removeVisited(Pair visited[], int *size, Pair pt){
    for(int i=0; i < *size; i++){
        if(visited[i].first == pt.first && visited[i].second == pt.second){
            visited[i] = visited[(*size)-1];
            (*size)--;
        }
    }
}


void printPath(Pair path[], int size){
    for(int it = 0; it < size; it++){
        printf("(%d, %d) -> ", path[it].first, path[it].second);
    }
    printf("MID");
}

// For searching in all 4 direction
int row[] = {-1, 1, 0, 0};
int col[] = { 0, 0, -1, 1};

// Coordinates of 4 corners of matrix
int _row[] = { 0, 0, N-1, N-1};
int _col[] = { 0, N-1, 0, N-1};


void findPathInMazeUtil(int maze[N][N], 
            Pair path[], Pair visited[], 
            Pair curr[], int *sizepath,
            int *sizevisited){
    if(curr->first == N/2 && curr->second == N/2){
        printPath(path, *sizepath);
        return;
    }

    for(int i=0; i < 4; i++){
        int n = maze[curr->first][curr->second];

        int x = curr->first + row[i]*n;
        int y = curr->second + col[i]*n;

        Pair next = {x,y};
        
        if(isValid(visited, *sizevisited, next)){

            //Marcamos la celda como visitada
            addVisited(visited, sizevisited, next);

            //Agregamos la celda al path actual
            path[*sizepath] = next;
            (*sizepath)++;

            //Llamamos recursivamente a la funcion para la celda siguiente
            findPathInMazeUtil(maze, path, visited, sizepath, sizevisited, &next);

            //Retrocedemos una celda. Aqui funciona el backtracking
            (*sizepath)--;
            removeVisited(visited, sizevisited, next);
            return;
        }
    }
}


int main(){
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
    //
    //findPathInMaze(maze);
    return 0;
}