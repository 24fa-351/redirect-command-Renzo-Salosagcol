#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <inp> <cmd> <out>\n", argv[0]);
        return 1;
    }

    int file_descriptors[2];
    pipe(file_descriptors);
    pid_t pid = fork();

    if (pid == 0) {
        close(file_descriptors[1]);

        int output_file = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        char buffer[100];
        ssize_t bytesRead;
        while ((bytesRead = read(file_descriptors[0], buffer, sizeof(buffer))) > 0) {
            write(output_file, buffer, bytesRead);
        }

        close(file_descriptors[0]);
        close(output_file);
    } else {
        close(file_descriptors[0]);

        int input_file = open(argv[1], O_RDONLY);
        char buffer[100];
        ssize_t bytesRead;
        while ((bytesRead = read(input_file, buffer, sizeof(buffer))) > 0) {
            write(file_descriptors[1], buffer, bytesRead);
        }

        close(file_descriptors[1]);
        close(input_file);

        wait(NULL);
    }

    return 0;
}
