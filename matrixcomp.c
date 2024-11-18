#include <stdio.h>
#include <stdlib.h>

// Función para multiplicar matrices cuadradas usando punteros
void multiply_matrices(int *A, int *B, int *C, int dim) {
    int i, j, k;

    // Inicializar la matriz resultado C con ceros
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            *(C + i * dim + j) = 0;
        }
    }

    // Multiplicar A y B, y almacenar el resultado en C
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            for (k = 0; k < dim; k++) {
                *(C + i * dim + j) += *(A + i * dim + k) * *(B + k * dim + j);
            }
        }
    }
}

// Función para imprimir una matriz cuadrada representada como un puntero
void print_matrix(int *M, int dim) {
    int i, j;
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            printf("%d ", *(M + i * dim + j));
        }
        printf("\n");
    }
}

int main() {
    int dim = 4;

    // Declaración de matrices como arreglos unidimensionales
    int A[] = {
        4, 6, 9, 2,
        1, 6, 8, 2,
        2, 5, 9, 4,
        1, 1, 7, 5
    };

    int B[] = {
        3, 6, 9, 1,
        2, 2, 1, 8,
        9, 7, 5, 2,
        1, 7, 6, 5
    };

    int C[dim * dim]; // Matriz resultado

    // Llamar a la función para multiplicar matrices
    multiply_matrices(A, B, C, dim);

    // Imprimir la matriz resultado
    printf("Matriz resultante:\n");
    print_matrix(C, dim);

    return 0;
}
