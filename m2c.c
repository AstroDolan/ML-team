#include <stdbool.h> // para usar el tipo bool, true, y false
#include <stdio.h>
#include <stdlib.h>

// Definir el tamaño de la cuadrícula
#define N 9
#define MAX_SIZE 100 // Tamaño máximo para `path` y `visited`
typedef struct {
    int first;
    int second;
} Pair;

int _row[] = {0, 0, N - 1, N - 1};
int _col[] = {0, N - 1, 0, N - 1};


//REVISAR SI AQUI ES NECESARIO USAR *SIZE EN VEZ DE SIZE
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
    int maze[N][N] =
	{
		{ 3, 5, 4, 4, 7, 3, 4, 6, 3 },
		{ 6, 7, 5, 6, 6, 2, 6, 6, 2 },
		{ 3, 3, 4, 3, 2, 5, 4, 7, 2 },
		{ 6, 5, 5, 1, 2, 3, 6, 5, 6 },
		{ 3, 3, 4, 3, 0, 1, 4, 3, 4 },
		{ 3, 5, 4, 3, 2, 2, 3, 3, 5 },
		{ 3, 5, 4, 3, 2, 6, 4, 4, 3 },
		{ 3, 5, 1, 3, 7, 5, 3, 6, 4 },
		{ 6, 2, 4, 3, 4, 5, 4, 5, 1 }
	};

	findPathInMaze(maze);
    return 0;
}