#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int n = atoi(argv[1]); // Number of child processes to create

  // Create an array of pipes for each child process
  int pipes[n][2];
  // for(int i = 0; i < argc; i++){
  //   printf("args: %s\n", argv[i]);
  // }

  for (int i = 0; i < n; i++) {
    if (pipe(pipes[i]) == -1) {
      return 1;
    }
  }

  for (int i = 0; i < n; i++) {
    int pid = fork();

    if (pid == 0) {       // Child process
      close(pipes[i][0]); // Close read end

      struct timeval start, end;
      gettimeofday(&start, NULL);

      int devNull = open("/dev/null", O_WRONLY); //stdout to /dev/null
      dup2(devNull, STDOUT_FILENO);
      dup2(devNull, STDERR_FILENO);


      char* commands[argc -2];  // commands to execute
      for (int i = 2; i < argc; i++) {
        commands[i-2] = argv[i];
      }
      commands[argc-2] = NULL;


      execvp(commands[0], commands);

      // gettimeofday(&end, NULL);
      // double elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
      // printf("Child %d execution time: %.3f milliseconds\n", getpid(), elapsedTime);
      // write(pipes[i][1], &elapsedTime, sizeof(double));

      // close(pipes[i][1]);
      
      close(devNull);

      exit(0);
    }
    else{//parent
    
      struct timeval start, end;
      gettimeofday(&start, NULL);
      waitpid(pid, NULL, 0);
      gettimeofday(&end, NULL);
      double elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
      write(pipes[i][1], &elapsedTime, sizeof(double));
      close(pipes[i][1]);
  
      

    }
  }

  // Parent wait
  double minTime = 100;
  double maxTime = 0;
  double totalTime = 0;

  for (int i = 0; i < n; i++) {
    close(pipes[i][1]); // Close write end

    int status;
    waitpid(-1, &status, 0);

    double elapsedTime;
    read(pipes[i][0], &elapsedTime, sizeof(double));
    printf("Child %d execution time: %.3f milliseconds\n",i+1, elapsedTime);
    
    if (elapsedTime < minTime) {
      minTime = elapsedTime;
    }
    if (elapsedTime > maxTime) {
      maxTime = elapsedTime;
    }
    totalTime += elapsedTime;

    close(pipes[i][0]); // Close read end
  }

  double averageTime = totalTime / n;
  printf("Max: %.3f millis\n", maxTime);
  printf("Min: %.3f millis\n", minTime);
  printf("Average: %.3f millis\n", averageTime);

  return 0;
}
