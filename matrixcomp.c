#include <stdio.h>

void multiplicarMatrices(int n, int matriz1[n][n], int matriz2[n][n], int resultado[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            resultado[i][j] = 0;
            for (int k = 0; k < n; k++) {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }
}

void imprimirMatriz(int n, int matriz[n][n], const char *nombre) {
    printf("Matriz %s:\n", nombre);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // Declaración de matrices directamente en el código
    int n = 3;  // Tamaño de la matriz
    int matriz1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    int matriz2[3][3] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };
    int resultado[3][3];

    // Llamada a la función de multiplicación
    multiplicarMatrices(n, matriz1, matriz2, resultado);

    // Impresión de resultados
    imprimirMatriz(n, matriz1, "A");
    imprimirMatriz(n, matriz2, "B");
    imprimirMatriz(n, resultado, "Resultado");

    return 0;
}