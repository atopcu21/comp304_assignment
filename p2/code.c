#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);        // Number of child processes to create
    char* command = "echo hello"; // Command to execute
    int childOrder =1;            // Counter for child processes
    int fd[2];                    // Pipe file descriptor

    if(pipe(fd) == -1) {
        return 1;
    }

    for (int i = 0; i < n; i++) {
        int pid = fork();

        if (pid == 0) {
            // Child process
            close(fd[0]);
            //printf("Child %d executing command: %s\n", getpid(), command);
            
            struct timeval start, end;
            gettimeofday(&start, NULL);
            
            // Redirect stdout and stderr to /dev/null

            int devNull = open("/dev/null", O_WRONLY);
            dup2(devNull, STDOUT_FILENO);
            dup2(devNull, STDERR_FILENO);
            
            system(command);
            
            gettimeofday(&end, NULL);
            double elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
            printf("Child %d execution time: %.3f milliseconds\n", getpid(), elapsedTime);
            write(fd[1], &elapsedTime, sizeof(double));
            
            close(fd[1]);
            close(devNull);

            exit(0);
        }
    }
    // Parent wait
    for (int i = 0; i < n; i++) {
        close(fd[1]);
        int status;
        waitpid(-1, &status, 0);

        double elapsedTime;
        read(fd[0], &elapsedTime, sizeof(double));
        
        printf("Child %d execution time: %.3f milliseconds\n", getpid(), elapsedTime);
        
        close(fd[0]);
    }

    return 0;
}