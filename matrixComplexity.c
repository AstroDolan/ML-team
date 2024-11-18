#include <stdio.h>
#include <stdlib.h>
#define N 4

/*
Este trabajo consta de multiplicar matrices de distintas formas
Primero con i,j luego j,i
Doble punteros, etc.
*/

int matrixMulti_rowcol(int dim, int A[dim][dim], int B[dim][dim], int C[dim][dim]);
int matrixMulti_colrow(int dim, int A[dim][dim], int B[dim][dim], int C[dim][dim]);
void print_rowcol(int dim, int C[dim][dim]);
void print_colrow(int dim, int C[dim][dim]);

int matrixMulti_pointer(int dim, int *A, int *B, int *C);
void print_pointer(int dim, int *C);

int main(){
    
    int dim = N; //Esto depende de mis matrices de entrada en el main

    int A[N][N] = {
        {4, 6, 9, 2},
        {1, 6, 8, 2},
        {2, 5 ,9, 4},
        {1, 1, 7, 5}
    };

    int B[N][N] = {
        {3, 6, 9, 1},
        {2, 2, 1, 8},
        {9, 7, 5, 2},
        {1, 7, 6, 5}
    };

    int C[dim][dim];

    matrixMulti_rowcol(dim, A, B, C);
    print_rowcol(dim, C);

    matrixMulti_colrow(dim, A, B, C);
    print_colrow(dim, C);

    matrixMulti_pointer(dim, *A, *B, *C);
    print_pointer(dim, *C);
    
    return 0;
}

int matrixMulti_rowcol(int dim, int A[dim][dim], int B[dim][dim], int C[dim][dim]){
    int i, j, k;
    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            C[i][j] = 0; //Inicializo la matriz
            for(k = 0; k < dim; k++){
                C[i][j] += A[i][k] * B[k][j]; //Asigno las multiplicaciones a la matriz C
            }
        }
    }
}

int matrixMulti_colrow(int dim, int A[dim][dim], int B[dim][dim], int C[dim][dim]){
    int i, j, k;
    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            C[i][j] = 0; //Inicializo la matriz
            for(k = 0; k < dim; k++){
                C[i][j] += A[j][k] * B[k][i]; //Asigno las multiplicaciones a la matriz C
            }
        }
    }
}

int matrixMulti_pointer(int dim, int *A, int *B, int *C){
    int i, j, k;
    
    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            *(C + i * dim + j) = 0; //Se inicializa C con solo ceros    
        }
    }
}


void print_rowcol(int dim, int C[dim][dim]){
    int i, j;
    printf("\nMatriz resultante de filas-columnas: \n");
    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
}

void print_colrow(int dim, int C[dim][dim]){
    int i, j;
    printf("\nMatriz resultante de columnas-filas: \n");
    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
}