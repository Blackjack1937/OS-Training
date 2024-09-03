#include <stdio.h>      // For printf()
#include <unistd.h>     // For fork(), execl()
#include <stdlib.h>     // For exit()
#include <sys/types.h>  // For pid_t

int main() {
    pid_t pid = fork();  // Create a new process

    if (pid < 0) {  // Error occurred during fork
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {  // Child process
        // Execute the /bin/ls command
        execl("/bin/ls", "ls", (char *)NULL);
        // If execl() fails
        perror("execl failed");
        exit(1);
    } else {  // Parent process
        printf("hello world\n");
    }

    return 0;
}
