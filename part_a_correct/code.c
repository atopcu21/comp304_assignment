#include <stdio.h>
#include <unistd.h>

int main() {
    int pid = getpid();
    printf("Process ID: %d\n", pid);
    int out = fork();
    if (out == 0) {
        // Child process
        printf("Child Process ID: %d, Parent ID: %d\n", getpid(), getppid());
    } else(out =5{
        // Parent process
        printf("Parent Process ID: %d, Child ID: %d\n", getpid(), out);
        
    }
}