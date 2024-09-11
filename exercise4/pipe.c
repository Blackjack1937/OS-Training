#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int pipefd[2];   // Array for pipe file descriptors: pipefd[0] is for reading, pipefd[1] is for writing
    pid_t pid;
    pid_t parent_pid;

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    // Fork a child process
    pid = fork();
    
    if (pid < 0) {
        // Fork failed
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process
        close(pipefd[1]);  // Close the write end of the pipe
        
        // Read the parent's PID from the pipe
        if (read(pipefd[0], &parent_pid, sizeof(parent_pid)) < 0) {
            perror("read");
            exit(1);
        }
        
        // Print the parent's PID (from the child's perspective)
        printf("Child: Parent's PID is %d\n", parent_pid);
        
        close(pipefd[0]);  // Close the read end of the pipe
    } else {
        // Parent process
        close(pipefd[0]);  // Close the read end of the pipe
        
        parent_pid = getpid();  // Get the parent's PID
        
        // Write the parent's PID to the pipe
        if (write(pipefd[1], &parent_pid, sizeof(parent_pid)) < 0) {
            perror("write");
            exit(1);
        }
        
        // Print the parent's PID (from the parent's perspective)
        printf("Parent: My PID is %d\n", parent_pid);
        
        close(pipefd[1]);  // Close the write end of the pipe
        
        // Optionally wait for the child to finish
        wait(NULL);
    }

    return 0;
}
