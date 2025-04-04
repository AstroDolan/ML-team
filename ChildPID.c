#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
    for(int i=0; i<3; i++){
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            printf("Soy el hijo %d (PID: %d) en ejecucion\n", i+1, getpid());
            sleep(i+1);
            exit(0);
        } else if (pid < 0) {
            
            perror("Fork failed");
            exit(1);
        }
    }

    for (int i=0; i<3; i++){
        int status;
        pid_t child_pid = wait(&status);
        if(WIFEXITED(status)){
            printf("El hijo con PID %d ha terminado con estado %d\n", child_pid, 
                WEXITSTATUS(status));
        } else {
            printf("El hijo con PID %d ha terminado de forma anormal\n", child_pid);
        }
    }
}