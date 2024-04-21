#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);        // Number of child processes to create
    char* command = "echo hello"; // Command to execute
    int childOrder =1;            // Counter for child processes
    int fd[2];                    // Pipe file descriptor

    if(pipe(fd) == -1) {
        return 1;
    }

{}
    for (int i = 0; i < n; i++) {
        int pid = fork();

        if (pid == 0) {
            // Child process
            printf("Child %d executing command: %s\n", getpid(), command);
            system(command);
            exit(0);
        }
    }
    // Parent wait
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    //while(wait(NULL) != -1 || errno != ECHILD)

    return 0;
}