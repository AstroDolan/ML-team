#include <stdio.h>
#include <stdbool.h>

#define N 9
#define MAX_SIZE 100 // Tamaño máximo para `path` y `visited`

typedef struct {
    int first;
    int second;
} Pair;

int _row[] = {0, 0, N - 1, N - 1};
int _col[] = {0, N - 1, 0, N - 1};

void addVisited(Pair visited[], int *size, Pair pt) {
    if (*size < MAX_SIZE) {
        visited[*size] = pt;
        (*size)++;
    }
}

void removeVisited(Pair visited[], int *size, Pair pt) {
    for (int i = 0; i < *size; i++) {
        if (visited[i].first == pt.first && visited[i].second == pt.second) {
            visited[i] = visited[(*size) - 1];
            (*size)--;
            break;
        }
    }
}

bool isVisited(Pair visited[], int size, Pair pt) {
    for (int i = 0; i < size; i++) {
        if (visited[i].first == pt.first && visited[i].second == pt.second) {
            return true;
        }
    }
    return false;
}

bool isValid(Pair visited[], int size, Pair pt) {
    return (pt.first >= 0) && (pt.first < N) &&
           (pt.second >= 0) && (pt.second < N) &&
           !isVisited(visited, size, pt);
}

void printPath(Pair path[], int pathSize) {
    for (int i = 0; i < pathSize; i++) {
        printf("(%d, %d) -> ", path[i].first, path[i].second);
    }
    printf("MID\n\n");
}

void findPathInMazeUtil(int maze[N][N], Pair path[], int *pathSize, Pair visited[], int *visitedSize, Pair *curr) {
    if (curr->first == N / 2 && curr->second == N / 2) {
        printPath(path, *pathSize);
        return;
    }

    int row[] = {-1, 1, 0, 0};
    int col[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        int n = maze[curr->first][curr->second];
        int x = curr->first + row[i] * n;
        int y = curr->second + col[i] * n;

        Pair next = {x, y};

        if (isValid(visited, *visitedSize, next)) {
            addVisited(visited, visitedSize, next);

            if (*pathSize < MAX_SIZE) {
                path[*pathSize] = next;
                (*pathSize)++;
            }

            findPathInMazeUtil(maze, path, pathSize, visited, visitedSize, &next);

            (*pathSize)--;
            removeVisited(visited, visitedSize, next);
        }
    }
}

void findPathInMaze(int maze[N][N]) {
    Pair path[MAX_SIZE];
    int pathSize = 0;

    Pair visited[MAX_SIZE];
    int visitedSize = 0;

    for (int i = 0; i < 4; ++i) {
        int x = _row[i];
        int y = _col[i];

        Pair pt = {x, y};

        addVisited(visited, &visitedSize, pt);

        if (pathSize < MAX_SIZE) {
            path[pathSize] = pt;
            pathSize++;
        }

        findPathInMazeUtil(maze, path, &pathSize, visited, &visitedSize, &pt);

        pathSize--;
        removeVisited(visited, &visitedSize, pt);
    }
}

int main() {
    int maze[N][N] = {
        { 3, 5, 4, 4, 7, 3, 4, 6, 3 },
        { 6, 7, 5, 6, 6, 2, 6, 6, 2 },
        { 3, 3, 4, 3, 2, 5, 4, 7, 2 },
        { 6, 5, 5, 1, 2, 3, 6, 5, 6 },
        { 3, 3, 4, 3, 0, 1, 4, 3, 4 },
        { 3, 5, 4, 3, 2, 2, 3, 3, 5 },
        { 3, 5, 4, 3, 2, 6, 4, 4, 3 },
        { 3, 5, 1, 3, 7, 5, 3, 6, 4 },
        { 6, 2, 4, 3, 4, 5, 4, 5, 1 }
    };

    findPathInMaze(maze);

    return 0;
}
