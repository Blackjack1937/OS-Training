#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *sourceFile, *destFile;
    char ch;

    // Open the source file (file1.txt) in read mode
    sourceFile = fopen("file1.txt", "r");
    if (sourceFile == NULL) {
        printf("Error: Could not open file1.txt for reading\n");
        exit(1);
    }

    // Open the destination file (file_copy.txt) in write mode
    destFile = fopen("file_copy.txt", "w");
    if (destFile == NULL) {
        printf("Error: Could not open file_copy.txt for writing\n");
        fclose(sourceFile);  // Close the source file if destination file can't be opened
        exit(1);
    }

    // Copy the content character by character from source to destination
    while ((ch = fgetc(sourceFile)) != EOF) {
        fputc(ch, destFile);
    }

    // Close the files
    fclose(sourceFile);
    fclose(destFile);

    printf("File copied successfully.\n");
    return 0;
}
