#include <stdio.h>
#include <stdlib.h>

//En ese codigo se cambia el bool por int 0 y 1 para simular booleanos
#define N 7
#define MAX_SIZE 49 //Tamaño máximo para path y visited

int size = 0;

typedef struct {
    int first;
    int second;
} Pair;

int _row[] = {0, 0, N - 1, N - 1};
int _col[] = {0, N - 1, 0, N - 1};

void fillMatrix(int dim, int maze[dim][dim]){
    int i, j;
    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            maze[i][j] = 1 + rand() % 2; //1 + rand() % (dim-1)
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
    for(int i=0; i < *size; i++){
        if(visited[i].first == pt.first && visited[i].second == pt.second){
            visited[i] = visited[(*size)-1];
            (*size)--;
            break;
        }
    }
}
// Función para comprobar si un punto está en el conjunto de visitados
int isVisited(Pair visited[], int size, Pair pt) {
    for (int i = 0; i < size; i++) {
        if (visited[i].first == pt.first && visited[i].second == pt.second) {
            return 1;
        }
    }
    return 0;
}

// Función para verificar si un punto es válido
int isValid(Pair visited[], int size, Pair pt) {
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

void findPathInMazeUtil(int maze[N][N], 
            Pair path[], Pair visited[], 
            Pair *curr, int *sizepath,
            int *sizevisited){
    if(curr->first == N/2 && curr->second == N/2){
        printPath(path, *sizepath);
        return;
    }

    // For searching in all 4 direction
    int row[] = {-1, 1, 0, 0};
    int col[] = {0, 0, -1, 1};

    for(int i=0; i < 4; i++){
        int n = maze[curr->first][curr->second];

        int x = curr->first + row[i]*n;
        int y = curr->second + col[i]*n;

        Pair next = {x,y};
        
        if(isValid(visited, *sizevisited, next)){

            //Marcamos la celda como visitada
            addVisited(visited, sizevisited, next);

            //Agregamos la celda al path actual
            if(*sizepath < MAX_SIZE){
                path[*sizepath] = next;
                (*sizepath)++;
            }
            
            //Llamamos recursivamente a la funcion para la celda siguiente
            findPathInMazeUtil(maze, path, visited, &next, sizepath, sizevisited);

            //Retrocedemos una celda. Aqui funciona el backtracking
            (*sizepath)--;
            removeVisited(visited, sizevisited, next);
        }
    }
}

void findPathInMaze(int maze[N][N]){
    Pair path[MAX_SIZE];
    int sizepath = 0;

    Pair visited[MAX_SIZE];
    int sizevisited = 0;

    for(int i=0; i<4; i++){
        int x = _row[i];
        int y = _col[i];
    
        Pair pt = {x,y};

        //Marcamos la celda como visitada
        addVisited(visited, &sizevisited, pt);

        //Agregamos la celda al path actual
        if(sizepath < MAX_SIZE){
            path[sizepath] = pt;
            (sizepath)++;
        }
        
        //Llamamos recursivamente a la funcion para la celda siguiente
        findPathInMazeUtil(maze, path, visited, &pt, &sizepath, &sizevisited);

        //Retrocedemos una celda. Aqui funciona el backtracking
        (sizepath)--;
        removeVisited(visited, &sizevisited, pt);
    }
}

int main(){
    // Array de puntos visitados
    int dim = N;
    int maze[dim][dim];

    fillMatrix(dim, maze);

	findPathInMaze(maze);
    return 0;
}