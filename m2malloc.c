#include <stdio.h>
#include <stdbool.h> // para usar el tipo bool, true, y false
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 451
#define MAX_SIZE 20000 //Tamaño máximo para path y visited
typedef struct {
    int first;
    int second;
} Pair;

int _row[] = {0, 0, N - 1, N - 1};
int _col[] = {0, N - 1, 0, N - 1};

// Función para llenar la matriz
void fillMatrix(int dim, int **maze){
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            maze[i][j] = 1 + rand() % 9;
        }
    }
}

void addVisited(Pair visited[], int *size, Pair pt){
    if(*size < MAX_SIZE){
        visited[*size] = pt;
        (*size)++;
    }
}

void removeVisited(Pair visited[], int *size, Pair pt){
    for(int i = 0; i < *size; i++){
        if(visited[i].first == pt.first && visited[i].second == pt.second){
            visited[i] = visited[(*size)-1];
            (*size)--;
            break;
        }
    }
}

bool isVisited(Pair visited[], int size, Pair pt) {
    for (int i = 0; i < size; i++) {
        if (visited[i].first == pt.first && visited[i].second == pt.second) {
            return true;
        }
    }
    return false;
}

bool isValid(Pair visited[], int size, Pair pt) {
    return (pt.first >= 0) && (pt.first < N) &&
           (pt.second >= 0) && (pt.second < N) &&
           !isVisited(visited, size, pt);
}

void printPath(Pair path[], int sizepath){
    for(int it = 0; it < sizepath; it++){
        printf("(%d, %d) -> ", path[it].first, path[it].second);
    }
    printf("MID\n");
}

void findPathInMazeUtil(int **maze, Pair path[], Pair visited[], Pair *curr, int *sizepath, int *sizevisited){
    if(curr->first == N/2 && curr->second == N/2){
        printPath(path, *sizepath);
        return;
    }

    int row[] = {-1, 1, 0, 0};
    int col[] = {0, 0, -1, 1};

    for(int i = 0; i < 4; i++){
        int n = maze[curr->first][curr->second];
        int x = curr->first + row[i] * n;
        int y = curr->second + col[i] * n;

        Pair next = {x, y};
        
        if(isValid(visited, *sizevisited, next)){
            addVisited(visited, sizevisited, next);

            if(*sizepath < MAX_SIZE){
                path[*sizepath] = next;
                (*sizepath)++;
            }
            
            findPathInMazeUtil(maze, path, visited, &next, sizepath, sizevisited);

            (*sizepath)--;
            removeVisited(visited, sizevisited, next);
        }
    }
}

void findPathInMaze(int **maze){
    Pair path[MAX_SIZE];
    int sizepath = 0;

    Pair visited[MAX_SIZE];
    int sizevisited = 0;

    for(int i = 0; i < 4; i++){
        int x = _row[i];
        int y = _col[i];
    
        Pair pt = {x, y};
        addVisited(visited, &sizevisited, pt);

        if(sizepath < MAX_SIZE){
            path[sizepath] = pt;
            sizepath++;
        }
        
        findPathInMazeUtil(maze, path, visited, &pt, &sizepath, &sizevisited);

        sizepath--;
        removeVisited(visited, &sizevisited, pt);
    }
}

int main(){
    // Memoria dinámica para la matriz
    int **maze = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        maze[i] = malloc(N * sizeof(int));
    }

    // Llenar la matriz
    fillMatrix(N, maze);

    // Ejecutar el algoritmo
    clock_t t;
    t = clock();
    findPathInMaze(maze);
    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("\n\nTomo %f segundos\n", time_taken);

    // Liberar memoria dinámica
    for (int i = 0; i < N; i++) {
        free(maze[i]);
    }
    free(maze);

    return 0;
}