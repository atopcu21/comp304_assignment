#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pid = fork();
    if (pid == 0) {// Child process
        printf("Child Process ID: %d, Parent ID: %d\n", getpid(), getppid());
        exit(0);
    }
    else {// Parent process
        sleep(5); 
    }

    return 0;
}