#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define EMPTY     0
#define NORMAL    1
#define FULL      2
#define NOCHIP    0
#define NOTHING   0
#define CHIP1     1
#define CHIP2     2
#define NOOWNER   3
#define NOCHIPT   ' '
#define CHIP1T    '*'
#define CHIP2T    'O'
#define UNVISITED 1
#define VISITED   0
#define SIMBOLIC  'S'
#define TEXTUAL   'T'

MPI_Status status;
int messages_sent = 0;
int messages_received = 0;

struct GOCell {
   unsigned char element, statuscell, prop;
};       

struct Queue {
   unsigned int *memarea, begin, end, qsize;
};  

struct Queue *InitQueue(unsigned int size) {
   struct Queue *q;

   q = calloc(1, sizeof(struct Queue));
   q->memarea = calloc(size, sizeof(unsigned int));
   q->qsize = size;
   return q;
}

unsigned char StatusQueue(struct Queue *q) {
    if (q->begin == q->end) 
        return EMPTY;
    if ((q->end + 1) % q->qsize == q->begin) 
        return FULL;
    return NORMAL;
}

void EnQueue(struct Queue *q, unsigned int pos) {
   q->end = (q->end + 1) % q->qsize;
   q->memarea[q->end] = pos;
}

void DeQueue(struct Queue *q, unsigned int *pos) {
   q->begin = (q->begin + 1) % q->qsize;
   *pos = q->memarea[q->begin];
}

unsigned int Owner(unsigned int e, unsigned int statusarea) {
   if ((e == CHIP1 || e == CHIP2) && statusarea == NOTHING) 
    return e;
   if (e == CHIP1 && statusarea == CHIP2) 
    return NOOWNER;
   if (e == CHIP2 && statusarea == CHIP1) 
    return NOOWNER;
   return statusarea;
}

unsigned int Analyze(struct GOCell **goboard, unsigned int size, unsigned int x, unsigned int y, struct Queue *q, unsigned int statusarea) {
   unsigned int pos;
   
   if (goboard[x][y].statuscell == UNVISITED) {
      if (goboard[x][y].element == NOCHIP) {
         pos = x * size + y;
         EnQueue(q, pos);
      } else {
         statusarea = Owner(goboard[x][y].element, statusarea);
      }
   }
   return statusarea;
}

int AnNeighb(struct GOCell **goboard, unsigned int size, struct Queue *q, unsigned int *lregion, unsigned int *statusarea) {
    unsigned int a, x, y, pos;

    a = 0;
    while (StatusQueue(q) != EMPTY) {
       DeQueue(q, &pos);
       x = pos / size;
       y = pos % size;
       lregion[0] += 1;
       lregion[lregion[0]] = pos;
       if (goboard[x][y].statuscell == UNVISITED) {
          a++;
          goboard[x][y].statuscell = VISITED;
          *statusarea = Analyze(goboard, size, x - 1, y, q, *statusarea);
          *statusarea = Analyze(goboard, size, x, y + 1, q, *statusarea);
          *statusarea = Analyze(goboard, size, x + 1, y, q, *statusarea);
          *statusarea = Analyze(goboard, size, x, y - 1, q, *statusarea);
       }
    }
    printf("Proceso %d: AnNeighb encontró %d celdas\n", a);
    return a;
}

void Process(struct GOCell **goboard, unsigned int n, int rank, int size) {
    unsigned int i, j, a, pos, *lregion, x, y;
    unsigned int statusarea;
    struct Queue *q;

    q = InitQueue(n * n);
    lregion = calloc(n * n + 1, sizeof(unsigned int));
    statusarea = NOTHING;

    int start = (n / size) * rank + 1;
    int end = (n / size) * (rank + 1);
    if (rank == size - 1) end = n - 2;

    printf("Proceso %d: Procesando filas desde %d hasta %d\n", rank, start, end);

    for (i = start; i <= end; i++) {
       for (j = 1; j <= n - 2; j++) {
          if (goboard[i][j].element == NOCHIP && goboard[i][j].statuscell == UNVISITED) {
             pos = i * n + j;
             EnQueue(q, pos);
             a = AnNeighb(goboard, n, q, lregion, &statusarea);
             MPI_Send(&a, 1, MPI_UNSIGNED, 0, 1, MPI_COMM_WORLD);
             messages_sent++;
             printf("Proceso %d: Enviando mensaje con valor %d\n", rank, a);
             printf("Area = %d Owner = %d\n", a, statusarea);
          }
       }
    }

    free(lregion);
    free(q->memarea);
    free(q);
}

void PrintBoard(struct GOCell **goboard, unsigned int n) {
    for (unsigned int i = 1; i <= n; i++) {
        for (unsigned int j = 1; j <= n; j++) {
            printf("%d,%d ", goboard[i][j].element, goboard[i][j].statuscell);
        }
        printf("\n");
    }
}

void PrintBoardSymbols(struct GOCell **goboard, unsigned int n) {
    for (unsigned int i = 1; i <= n; i++) {
        for (unsigned int j = 1; j <= n; j++) {
            if (goboard[i][j].element == CHIP1) {
                printf("*");
            } else if (goboard[i][j].element == CHIP2) {
                printf("O");
            } else {
                printf("%d", goboard[i][j].statuscell);
            }
        }
        printf("\n");
    }
}

void ReadGOBoard(struct GOCell **goboard, unsigned int n, int rank, int size) {
    unsigned int i, j;
    unsigned int rows_per_process = n / size;
    unsigned int start_row = rank * rows_per_process + 1;
    unsigned int end_row = (rank == size - 1) ? n : start_row + rows_per_process - 1;

    for (i = start_row; i <= end_row; i++) {
        for (j = 1; j <= n; j++) {
            if (rank == 0) {
                scanf("%1d", &goboard[i][j].element);
            }
            MPI_Bcast(&goboard[i][j].element, 1, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
            goboard[i][j].statuscell = UNVISITED;
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size, a = 0; // Inicializar 'a' a 0
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    unsigned int n, i;
    struct GOCell **goboard;

    if (rank == 0) {
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            fprintf(stderr, "Error al abrir el archivo\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fscanf(file, "%d", &n);
        fclose(file);
    }
    MPI_Bcast(&n, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    goboard = calloc(n + 2, sizeof(struct GOCell *));
    for (i = 0; i < n + 2; i++)
        goboard[i] = calloc(n + 2, sizeof(struct GOCell));

    if (rank == 0) {
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            fprintf(stderr, "Error al abrir el archivo\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fscanf(file, "%d", &n); // Leer el tamaño del tablero nuevamente
        for (i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                fscanf(file, "%1d", &goboard[i][j].element);
            }
        }
        fclose(file);
        for (int dest = 1; dest < size; dest++) {
            for (i = 0; i < n + 2; i++) {
                MPI_Send(goboard[i], (n + 2) * sizeof(struct GOCell), MPI_BYTE, dest, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        for (i = 0; i < n + 2; i++) {
            MPI_Recv(goboard[i], (n + 2) * sizeof(struct GOCell), MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    printf("Proceso %d: Iniciando procesamiento\n", rank);
    Process(goboard, n + 2, rank, size);
    printf("Proceso %d: Procesamiento terminado\n", rank);

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Recv(&a, 1, MPI_UNSIGNED, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Proceso 0: Recibido mensaje con valor %d de proceso %d\n", a, i);
        }
        printf("Tablero final:\n");
        PrintBoard(goboard, n);
        printf("\nTablero con símbolos:\n");
        PrintBoardSymbols(goboard, n);
    } else {
        MPI_Send(&a, 1, MPI_UNSIGNED, 0, 1, MPI_COMM_WORLD);
        printf("Proceso %d: Enviado mensaje con valor %d al proceso 0\n", rank, a);
    }

    for (i = 0; i < n + 2; i++) {
        free(goboard[i]);
    }
    free(goboard);

    MPI_Finalize();
    return 0;
}