#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define MAX_SIZE 1000

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <number_to_find> <number_of_processes>\n", argv[0]);
    exit(1);
  }

  int target = atoi(argv[1]);
  int num_processes = atoi(argv[2]);

  int shmid;
  key_t key = IPC_PRIVATE; // Get a unique key for shared memory
  size_t shmsize = sizeof(int); // Shared memory will hold the found index (or -1)

  // Create shared memory
  if ((shmid = shmget(key, shmsize, IPC_CREAT | 0666)) < 0) {
    perror("shmget");
    exit(1);
  }

  int *shared_index = (int *)shmat(shmid, NULL, 0); // Attach to shared memory

  // Read sequence from stdin
  int sequence[MAX_SIZE];
  int num_elements = 0;
  while (scanf("%d", &sequence[num_elements]) != EOF && num_elements < MAX_SIZE) {
    num_elements++;
  }

  int elements_per_process = num_elements / num_processes;
  int remaining_elements = num_elements % num_processes;

  // Create child processes
  for (int i = 0; i < num_processes; i++) {
    pid_t pid = fork();
    if (pid < 0) {
      perror("fork");
      exit(1);
    } else if (pid == 0) { // Child process
      int start_index = i * elements_per_process;
      int end_index = (i == num_processes - 1) ? num_elements : start_index + elements_per_process;

      // Search for target in assigned portion
      for (int j = start_index; j < end_index; j++) {
        if (sequence[j] == target) {
          *shared_index = j; // Update shared index with found position
          shmdt(shared_index); // Detach from shared memory
          exit(0); // Child exits successfully
        }
      }

      // Not found in this portion
      shmdt(shared_index);
      exit(1); // Child exits with failure
    }
  }

  // Parent process waits and checks results
  int status;
  while (wait(&status) > 0) {
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
      printf("Found at index: %d\n", *shared_index);
      shmdt(shared_index); // Detach from shared memory
      shmctl(shmid, IPC_RMID, NULL); // Remove shared memory
      for (int i = 0; i < num_processes - 1; i++) { // Kill remaining children
        kill(waitpid(-1, NULL, WNOHANG), SIGKILL);
      }
      exit(0); // Parent exits successfully
    }
  }

  // No child found the target
  printf("Not found\n");
  shmdt(shared_index);
  shmctl(shmid, IPC_RMID, NULL);
  exit(1); // Parent exits with failure
}
