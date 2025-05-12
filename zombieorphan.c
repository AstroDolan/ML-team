#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int status, value;

    pid = fork();

    if (pid > 0) { // Father

        pid = fork();

        if (pid > 0) { // Father
            wait(&status);
            value = WEXITSTATUS(status);

            if (value == 2)
                printf("Child 2");
            else if (value == 3)
                printf("Child 1");

        } else if (pid == 0) { //Child 2 - Orphan
            sleep(4);
            exit(2);

        } else {
            exit(1);
        }

    } else if (pid == 0) { // Child 1 - Zombie
        sleep(1);
        exit(3);

    } else {
        printf("Error al ejecutar el fork");
        exit(1);
    }



    return 0;
}