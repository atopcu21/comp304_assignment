#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if(argc <2){
        return 1;
    }
    else{
        int lvlCounter = 1;
        int input = atoi(argv[1]);
        printf("Main Process ID: %d, Parent ID: %d, level: %d\n",getpid(), getppid(), 0);
        for(int i = 0; i < input; i++){
            int id = fork();
            if(id == 0){
                //printf("child\n");
                printf("Process ID: %d, Parent ID: %d, level: %d\n",getpid(), getppid(), lvlCounter++);
                //printf("child\n");
            }
            //printf("Process ID: %d, Parent ID: %d, level: %d\n",getpid(), getppid(), lvlCounter++);
        }
    }
}
