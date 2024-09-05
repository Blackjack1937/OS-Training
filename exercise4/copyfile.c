#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    int dest_fd;
    char buf[512];
    int nbytes;

    if ((fd = open("file1.txt", O_RDONLY))< 0) {
        perror("open");
        exit(1);
    }

    if ((dest_fd = open("filecopy.txt", O_WRONLY | O_CREAT, S_IRUSR)) < 0) {
        perror("open destination file");
        exit(1);
    }

     while ((nbytes = read(fd, buf, sizeof(buf))) > 0) {
        if (write(dest_fd, buf, nbytes) != nbytes) {
            perror("write");
            exit(1);
        }
    }

    if (nbytes < 0) {  
        exit(1);
    }

    
    close(fd);
    close(dest_fd);

    printf("File copied successfully.\n");

    return 0;
}