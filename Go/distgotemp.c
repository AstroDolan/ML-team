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
   if (q->begin == q->end) return EMPTY;
   if ((q->end + 1) % q->qsize == q->begin) return FULL;
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
   if ((e == CHIP1 || e == CHIP2) && statusarea == NOTHING) return e;
   if (e == CHIP1 && statusarea == CHIP2) return NOOWNER;
   if (e == CHIP2 && statusarea == CHIP1) return NOOWNER;
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
   unsigned int a = 0, x, y, pos;
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

   for (i = start; i <= end; i++) {
      for (j = 1; j <= n - 2; j++) {
         if (goboard[i][j].element == NOCHIP && goboard[i][j].statuscell == UNVISITED) {
            pos = i * n + j;
            EnQueue(q, pos);
            a = AnNeighb(goboard, n, q, lregion, &statusarea);
            MPI_Send(&a, 1, MPI_UNSIGNED, 0, 1, MPI_COMM_WORLD);
            messages_sent++;
         }
      }
   }
}

int main(int argc, char *argv[]) {
   int rank, size, a;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   unsigned int n, i;
   struct GOCell **goboard;

   if (rank == 0) scanf("%d", &n);
   MPI_Bcast(&n, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
   messages_sent++;

   goboard = calloc(n + 2, sizeof(struct GOCell *));
   for (i = 0; i < n + 2; i++)
      goboard[i] = calloc(n + 2, sizeof(struct GOCell));

   if (rank == 0) ReadGOBoard(goboard, n);
   MPI_Bcast(&(goboard[0][0]), (n + 2) * (n + 2) * sizeof(struct GOCell), MPI_BYTE, 0, MPI_COMM_WORLD);
   messages_sent++;
   messages_received++;

   Process(goboard, n + 2, rank, size);

   if (rank == 0) {
      for (int i = 1; i < size; i++) {
         MPI_Recv(&a, 1, MPI_UNSIGNED, i, 1, MPI_COMM_WORLD, &status);
         messages_received++;
         printf("Received area = %d from rank %d\n", a, i);
      }
   }

   printf("Rank %d: Messages sent = %d, Messages received = %d\n", rank, messages_sent, messages_received);

   MPI_Finalize();
   return 0;
}
