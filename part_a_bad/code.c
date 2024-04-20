#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int childMaker(int input, int level){
    
        for(int i = 0; i < input; i++){
            int pid = fork();
            if(pid == 0){
                printf("Process ID: %d, Parent ID: %d, level: %d\n", getpid(), getppid(), level+1);
                exit(0);
            }

        }
        for(int i = 0; i < input; i++){
            int pid = wait(NULL);

        }
}

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Invalid number of arguments arcCount = %d\n", argc);
        return 1;
    }
    else{
        printf("Main Process ID: %d, Parent ID: %d, level: %d\n", getpid(), getppid(), 0);
        int input = atoi(argv[1]);
        for(int i = 0; i < input; i++){
            
            int pid = fork();
            if(pid == 0){
                printf("child Process ID: %d, Parent ID: %d, level: %d\n", getpid(), getppid(),i+1);
                childMaker(input, i);
                exit(0);
            }
            else{
                int pid = wait(NULL);
            }
            
        }
        

    }
}


