#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    int n = atoi(argv[1]);
    int level = 0; 
    
    printf("Main Process ID: %d, level: %d\n", getpid(), level);

    for (int i = 0; i < n; ++i) {
        int pid = fork();
        if (pid == 0) { // Child process
            printf("Process ID: %d, Parent ID: %d, level: %d\n", getpid(), getppid(), level + 1);
            level++;
        }
        else { // Parent process
            wait(NULL);
        }
    }

    return 0;
}