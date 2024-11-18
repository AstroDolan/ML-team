#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 500 //N varia segun el input de la matriz del main

/*
Este trabajo consta de multiplicar matrices de distintas formas
*/

//Aqui declaramos las funciones de multiplicacion basica fila-columna y columna-fila
int matrixMulti_rowcol(int dim, int A[dim][dim], int B[dim][dim], int C[dim][dim]);
int matrixMulti_colrow(int dim, int A[dim][dim], int B[dim][dim], int C[dim][dim]);
void print_rowcol(int dim, int C[dim][dim]);
void print_colrow(int dim, int C[dim][dim]);

//Estas son las funciones de multiplicacion con punteros
int matrixMulti_pointer(int dim, int *A, int *B, int *C);
void print_pointer(int dim, int *C);


//Multiplicacion fila-columna
int matrixMulti_rowcol(int dim, int A[dim][dim], int B[dim][dim], int C[dim][dim]){
    int i, j, k;
    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            C[i][j] = 0; //Se inicializa la matriz
            for(k = 0; k < dim; k++){
                C[i][j] += A[i][k] * B[k][j]; //Se asignan las multiplicaciones a la matriz C
            }
        }
    }
}

//Multiplicacion columna-fila
int matrixMulti_colrow(int dim, int A[dim][dim], int B[dim][dim], int C[dim][dim]){
    int i, j, k;
    for(j = 0; j < dim; j++){
        for(i = 0; i < dim; i++){
            C[i][j] = 0; //Se inicializa la matriz
            for(k = 0; k < dim; k++){
                C[i][j] += A[i][k] * B[k][j]; //Se asignan las multiplicaciones a la matriz C
            }
        }
    }
}

//Multiplicacion con punteros
int matrixMulti_pointer(int dim, int *A, int *B, int *C){
    int i, j, k;
    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            *(C + i * dim + j) = 0; //Se inicializa C con solo ceros    
        }
    }

    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            for(k = 0; k < dim; k++){
                *(C + i * dim + j) += *(A + i * dim + k) * *(B + k * dim + j);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////
             //FUNCIONES PARA IMPRIMIR EN CONSOLA//

             
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

void print_pointer(int dim, int *C){
    int i, j;
    printf("\nMatriz resultante de la multiplicacion por punteros: \n");
    for(i = 0; i < dim; i++){
        for(j = 0; j < dim; j++){
            printf("%d ", *(C + i * dim + j));
        }
        printf("\n");
    }
}

int main(){
    

    int dim = N; //Esto depende de mis matrices de entrada en el main

    int C[dim][dim];

    clock_t t1; 
    t1 = clock();
    matrixMulti_rowcol(dim, A, B, C);
    print_rowcol(dim, C);
    t1 = clock() - t1;
    double time_taken = ((double)t1)/CLOCKS_PER_SEC;
    printf("La multiplicacion de matrices por fila-columna tomo %f segundos en ser ejecutada \n", time_taken);

    matrixMulti_colrow(dim, A, B, C);
    //print_colrow(dim, C);

    clock_t t3;
    t3 = clock();
    matrixMulti_pointer(dim, *A, *B, *C);
    print_pointer(dim, *C);
    t3 = clock() - t3;
    double time_taken3 = ((double)t3)/CLOCKS_PER_SEC;
    printf("La multiplicacion de matrices con punteros tomo %f segundos en ser ejecutada \n", time_taken);
    
    return 0;
}
