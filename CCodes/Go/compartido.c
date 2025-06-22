#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define EMPTY     0
#define FULL      2
#define NOCHIP    0
#define CHIP1     1
#define CHIP2     2
#define NOOWNER   3
#define UNVISITED 1
#define VISITED   0

struct GOCell {
    unsigned char element, statuscell, prop;
};

void ReadGOBoard(struct GOCell **goboard, unsigned int n, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    for (unsigned int i = 0; i < n; ++i) {
        for (unsigned int j = 0; j < n; ++j) {
            fscanf(file, "%1hhu", &goboard[i][j].element);
            goboard[i][j].statuscell = UNVISITED;
        }
    }
    fclose(file);
}

void PrintGOBoard(struct GOCell **goboard, unsigned int n, int rank, int size, char partition_mode) {
    MPI_Barrier(MPI_COMM_WORLD);
    for (int r = 0; r < size; ++r) {
        if (rank == r) {
            printf("Process %d:\n", rank);
            for (unsigned int i = 0; i < ((partition_mode == 'H') ? n / size : n); ++i) {
                for (unsigned int j = 0; j < ((partition_mode == 'V') ? n / size : n); ++j) {
                    printf("%d ", goboard[i][j].element);
                }
                printf("\n");
            }
            printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

void CountChips(struct GOCell **subboard, unsigned int rows, unsigned int cols, unsigned int *chip1_count, unsigned int *chip2_count) {
    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < cols; ++j) {
            if (subboard[i][j].element == CHIP1) (*chip1_count)++;
            if (subboard[i][j].element == CHIP2) (*chip2_count)++;
        }
    }
}

void ProcessRegion(struct GOCell **subboard, unsigned int rows, unsigned int cols, unsigned int rank) {
    unsigned int chip1_count = 0, chip2_count = 0;

    CountChips(subboard, rows, cols, &chip1_count, &chip2_count);

    unsigned int total_chip1, total_chip2;
    MPI_Reduce(&chip1_count, &total_chip1, 1, MPI_UNSIGNED, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&chip2_count, &total_chip2, 1, MPI_UNSIGNED, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total CHIP1=%d, CHIP2=%d\n", total_chip1, total_chip2);
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 4) {
        if (rank == 0) {
            fprintf(stderr, "Usage: %s -P[V|H] -M[S|V] <filename>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    char partition_mode = argv[1][2];
    const char *filename = argv[3];

    unsigned int n;
    struct GOCell **goboard = NULL;
    if (rank == 0) {
        FILE *file = fopen(filename, "r");
        if (!file) {
            fprintf(stderr, "Error opening file %s\n", filename);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fscanf(file, "%u", &n);
        fclose(file);

        goboard = malloc(n * sizeof(struct GOCell *));
        goboard[0] = malloc(n * n * sizeof(struct GOCell));
        for (unsigned int i = 1; i < n; ++i) {
            goboard[i] = goboard[0] + i * n;
        }
        ReadGOBoard(goboard, n, filename);
    }

    MPI_Bcast(&n, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    unsigned int local_rows = (partition_mode == 'H') ? n / size : n;
    unsigned int local_cols = (partition_mode == 'V') ? n / size : n;

    MPI_Datatype MPI_GOCell;
    MPI_Type_contiguous(3, MPI_UNSIGNED_CHAR, &MPI_GOCell);
    MPI_Type_commit(&MPI_GOCell);

    struct GOCell *subboard = malloc(local_rows * local_cols * sizeof(struct GOCell));

    MPI_Scatter(&goboard[0][0], local_rows * n, MPI_GOCell, subboard, local_rows * n, MPI_GOCell, 0, MPI_COMM_WORLD);

    ProcessRegion((struct GOCell **) &subboard, local_rows, local_cols, rank);

    free(subboard);

    if (rank == 0) {
        free(goboard[0]);
        free(goboard);
    }

    MPI_Type_free(&MPI_GOCell);
    MPI_Finalize();
    return 0;
}
