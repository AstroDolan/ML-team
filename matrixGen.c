#include<stdlib.h>
#include<stdio.h>
#include<time.h>

void fillMatrix(int dim, int maze[dim][dim]);
void showMatrix(int dim, int maze[dim][dim]);

int main(){
    srand(time(NULL));
    int dim;

    printf("Ingrese la dimension: \n");
    scanf("%d",&dim);
    int maze[dim][dim];

    fillMatrix(dim, maze);
    showMatrix(dim, maze);
    

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

void showMatrix(int dim, int maze[dim][dim]){
    int i, j;
    printf("int maze[%d][%d] = \n{\n", dim, dim);
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