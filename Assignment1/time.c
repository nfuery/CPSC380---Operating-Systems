#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Check if the command-line argument is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Get the current time before forking the process
    struct timeval current;
    gettimeofday(&current, NULL);
    
    // Fork the process
    pid_t pid = fork();

    // Check for fork failure
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } 
    // Child process
    else if (pid == 0) {
        char command[100];  // Variable to store the command
        strcpy(command, argv[1]);

        // Execute the command using execlp and check if valid command
        if (execlp(command, command, (char *)NULL) == -1) {
            perror("Exec failed");
            fprintf(stderr, "Error: Incorrect command or command not found.\n");
            exit(EXIT_FAILURE);
        }
    } 
    // Parent process
    else {
        // Wait for the child process to complete
        wait(NULL);

        // Get the current time after the child process completes
        struct timeval current2;
        gettimeofday(&current2, NULL);

        // Print the completion message and the time taken by the child process
        printf("Child process completed.\n");
        printf("Time: %ld microseconds\n", (current2.tv_sec - current.tv_sec) * 1000000 + current2.tv_usec - current.tv_usec);
    }

    return 0;
}
