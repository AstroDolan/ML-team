#include <stdio.h>
#include <stdlib.h>

#define N 10
#define M N*N

/*
Este trabajo consta de multiplicar matrices de distintas formas
Primero con i,j luego j,i
Doble punteros, etc.
*/

int matrixMulti_rowcol(int dim, int A[dim][dim], int B[dim][dim]);
int matrixMulti_colrow(int dim, int A[dim][dim], int B[dim][dim]);
void matrixReading(int C[M][M]);

int main(){
    int dim = 3; //Esto depende de mis matrices de entrada en el main

    int A[dim][dim] = {
        {4, 6, 9, 2},
        {1, 6, 8, 2},
        {2, 5 ,9, 4},
        {1, 1, 7, 5}
    }

    int B[dim][dim] = {
        {3, 6, 9, 1},
        {2, 2, 1, 8},
        {9, 7, 5, 2},
        {1, 7, 6, 5}
    }
    printf("matrix complexity: ");
    return 0;
}

int matrixMulti_rowcol(int dim, int A[dim][dim], int B[dim][dim], int C[dim][dim]){
    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            C[i][j] = 0; //Inicializo la matriz
            for(k = 0; k < dim; k++){
                C[i][j] += A[i][k] * B[k][j]; //Asigno las multiplicaciones a la matriz C
            }
        }
    }
};

void print_rowcol(int dim, int C[i][j]){
    printf("Matriz resultante: ");
    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            printf("%d", C[i][j]);
            if(j < dim - 1){
                printf("\n");
            }
        }
    }
}

int matrixMulti_rowcol(int dim, int A[dim][dim], int B[dim][dim]){
    int i, j;
    for(j = 0; j < dim; j++){
        for(i = 0; i < dim; i++){
            printf("pao hermosa");
        }
    }
};