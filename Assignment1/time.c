#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>   
#include <stdlib.h>   

int main(int argc, char *argv[]){
    struct timeval current;
    gettimeofday(&current,NULL);    
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(0);
    } else if (pid == 0) {
        char *command;
        strcpy(command, argv[1]);
        if (execlp(command, command, (char *)NULL) == -1) {
            perror("Exec failed");
            exit(0);
        }
    } else {
        wait(NULL);
        printf("Child process completed.\n");
    }
    struct timeval current2;
    gettimeofday(&current2, NULL);
    printf("Time: %ld\n", (current2.tv_sec-current.tv_sec)*1000000 + current2.tv_usec-current.tv_usec);
    return 0;
}