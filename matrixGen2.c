#include<stdlib.h>
#include<stdio.h>
#include<time.h>

/*Workflow
Matriz minima para ver segundos, posiblemente M20
M50  ****************************
M80  ****************************
M100 ****************************
M150 ****************************
M200 ****************************
M300 ****************************
M400 ****************************
M500 int 1-300 o 1-100 **********
Ver si es necesario seguir con M600 y M700

Se debe compilar tres veces por Mx y validar con 3 compilaciones M_inicial
Las compilaciones deben tener por nombre mCx, x = N
M400.txt funciona para mC400

*/

void fillMatrix(int dim, int maze[dim][dim]);
void fillMatrixB(int dim, int mazeB[dim][dim]);
void showMatrix(int dim, int maze[dim][dim]);
void showMatrixB(int dim, int mazeB[dim][dim]);

int main(){
    srand(time(NULL));
    int dim;

    printf("Ingrese la dimension: \n");
    scanf("%d",&dim);
    int maze[dim][dim];
    int mazeB[dim][dim];

    fillMatrix(dim, maze);
    fillMatrix(dim, mazeB);
    showMatrix(dim, maze);
    showMatrixB(dim, mazeB);


    return 0;
}

void fillMatrix(int dim, int maze[dim][dim]){
    int i, j;
    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            maze[i][j] = 1 + rand() % (dim-1); //1 + rand() % (dim-1)
        }
    }
}

void fillMatrixB(int dim, int mazeB[dim][dim]){
    int i, j;
    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            mazeB[i][j] = 1 + rand() % (dim-1); //1 + rand() % (dim-1)
        }
    }
}

void showMatrix(int dim, int maze[dim][dim]){
    int i, j;
    printf("int A[%d][%d] = \n{\n", dim, dim);
    for(i = 0; i < dim; i++){
        printf("   { ");
        for(j = 0; j < dim; j++){
            printf("%d", maze[i][j]);
            if(j < dim - 1){
                printf(", ");
            }
        }
        printf(" }");
        if(i < dim - 1){
            printf(",");
        }
        printf("\n");
    }
    printf("};\n");
}

void showMatrixB(int dim, int mazeB[dim][dim]){
    int i, j;
    printf("\nint B[%d][%d] = \n{\n", dim, dim);
    for(i = 0; i < dim; i++){
        printf("   { ");
        for(j = 0; j < dim; j++){
            printf("%d", mazeB[i][j]);
            if(j < dim - 1){
                printf(", ");
            }
        }
        printf(" }");
        if(i < dim - 1){
            printf(",");
        }
        printf("\n");
    }
    printf("};\n");
}