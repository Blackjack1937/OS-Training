#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_processes>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {  // Child process
            sleep(1);  // Sleep for 1 second
            printf("Child PID: %d\n", getpid());
            exit(0);  // Exit the child process
        }
    }

    // Parent process waits for all child processes to terminate
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    return 0;
}
