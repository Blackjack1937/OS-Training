#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

typedef struct {
    int value;    // Random value
    pid_t pid;    // PID of the process that generated the random value
    int node;     // Node number (process number)
} Message;

int main(int argc, char *argv[]) {
    int n;  // Number of processes
    pid_t pid;
    int i;

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

    // Pipes: each process will have two ends (input and output pipe)
    int pipes[n][2];  // Array of pipes

    // Create pipes
    for (i = 0; i < n; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    // Fork n processes
    for (i = 0; i < n; i++) {
        pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // Child process

            // Seed the random number generator with a unique value for each process
            srand(time(NULL) + getpid());

            // Close all pipes except the ones the child needs
            for (int j = 0; j < n; j++) {
                if (j != i) close(pipes[j][0]);  // Close reading end of other pipes
                if (j != (i + 1) % n) close(pipes[j][1]);  // Close writing end of other pipes
            }

            // Generate random value
            int random_value = rand();
            Message msg;
            msg.value = random_value;
            msg.pid = getpid();
            msg.node = i;

            // Print the process info
            printf("Process pid %d node %d val = %d\n", msg.pid, msg.node, msg.value);

            // Receive message from the previous process
            if (i > 0) {
                read(pipes[i][0], &msg, sizeof(msg));

                // Compare and forward the larger value
                if (random_value > msg.value) {
                    msg.value = random_value;
                    msg.pid = getpid();
                    msg.node = i;
                }
            }

            // Send message to the next process in the ring
            write(pipes[(i + 1) % n][1], &msg, sizeof(msg));

            // Close used pipes
            close(pipes[i][0]);
            close(pipes[(i + 1) % n][1]);

            exit(0);  // Child process exits after handling its part
        }
    }

    // Parent process (Process 0)

    // Close unnecessary pipes in the parent process
    for (int j = 1; j < n; j++) {
        close(pipes[j][0]);  // Close reading ends of other pipes
        close(pipes[j][1]);  // Close writing ends of other pipes
    }

    // Process 0 receives the message with the largest value from the last process
    Message final_msg;
    read(pipes[0][0], &final_msg, sizeof(final_msg));

    // Close the pipe
    close(pipes[0][0]);

    // Print the winner
    printf("The winner is %d, pid %d, node %d\n", final_msg.value, final_msg.pid, final_msg.node);

    // Wait for all child processes to finish
    for (i = 0; i < n; i++) {
        wait(NULL);
    }

    return 0;
}
