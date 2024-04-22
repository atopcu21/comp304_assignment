#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: ./p3 x n where x=given num & n is child count\n");
    return 1;
  }
  int SHM_SIZE = 1024;
  int max_nums = 1000;

  int x = atoi(argv[1]);
  int n = atoi(argv[2]);

  int seq[max_nums];
  int numCount = 0;

  char buffer[100];                                       //stdin
  while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    seq[numCount++] = atoi(buffer);
    if (numCount >= max_nums) {
      break;
    }
  }

  int portionSize = numCount / n; // calculate portion size for child

  
  int shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666); //create shared memory segment

  int *sharedSeq = (int *)shmat(shmid, NULL, 0);

  for (int i = 0; i < numCount; i++) { ////copy seq to shared memory
    sharedSeq[i] = seq[i];
  }

  int childPids[n];
  for (int i = 0; i < n; i++) {
    int end;
    int start;
    int pid = fork();
    if (pid == 0) { // child process
      int *sharedSeq = (int *)shmat(shmid, NULL, 0); //attach to shared memory segment

      start = i * portionSize;
      if (i == n - 1) {
        end = numCount;
      } else {
        end = start + portionSize;
      }

      for (int j = start; j < end; j++) {
        if (sharedSeq[j] == x) {
          printf("given x =%d found at index %d\n", x,j);
          shmdt(sharedSeq); //detach from shared memory segment
          exit(0);
        }
      }

      shmdt(sharedSeq);     //detach from shared memory segment
      exit(1);
    } else { //parent
      childPids[i] = pid;
    }
  }

  for (int i = 0; i < n; i++) {
    int status;
    waitpid(childPids[i], &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
      break;
    } else if (i == n - 1) {
      printf("program did not find the number\n");
    }
  }

  shmdt(sharedSeq);             //detach from shared memory segment
  shmctl(shmid, IPC_RMID, NULL);//remove shared memory segment

  return 0;
}