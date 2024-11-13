#include<stdio.h>
#include<time.h>

int main(){
    srand(time(NULL));
    int dim;

    printf("Ingrese la dimension: ");
    scanf("d",&dim);
    int maze[dim][dim];

    showMatrix(dim, maze);
    fillMatrix(dim, maze);

    return 0;
}

void showMatrix(int dim, int maze[dim][dim]){
    int i, j;
    for(i = 0; i < dim; i++){
        for(j = 0; i < dim; i++){
            printf("d", maze[i][j]);
        }
        printf("\n");
    }
}


void fillMatrix(int dim, int maze[dim][dim]){
    int i, j;
    for(i = 0; i < dim; i++){
        for(j = 0; i < dim; i++){
            maze[dim][dim] = 1 + rand() % 9;
        }
    }
}

