#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int n;  // Number of processes
    pid_t pid;

    // Check if the correct number of arguments is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_processes>\n", argv[0]);
        exit(1);
    }

    // Convert the argument to an integer
    n = atoi(argv[1]);

    if (n <= 0) {
        fprintf(stderr, "Number of processes should be a positive integer\n");
        exit(1);
    }

    // Loop to create n child processes
    for (int i = 0; i < n; i++) {
        pid = fork();

        if (pid < 0) {
            // Fork failed
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // Child process
            srand(time(NULL) + getpid());  // Seed the random number generator uniquely
            int random_val = rand();       // Generate a random number

            // Print the process details
            printf("Process pid %d node %d val = %d\n", getpid(), i, random_val);
            
            // Child process should exit after printing its information
            exit(0);
        }
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    return 0;
}
