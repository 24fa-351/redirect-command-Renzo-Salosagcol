#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute_command(char *input, char *command, char *output) {
    int input_file, output_file;

    if (strcmp(input, "-") != 0) {
        inp_fd = open(input, O_RDONLY);
        if (inp_fd == -1) {
            perror("open input file");
            exit(EXIT_FAILURE);
        }
        dup2(inp_fd, STDIN_FILENO);
        close(inp_fd);
    }

    if (strcmp(out, "-") != 0) {
        out_fd = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out_fd == -1) {
            perror("open output file");
            exit(EXIT_FAILURE);
        }
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }

    char *args[20];
    char *token = strtok(cmd, " ");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (execvp(args[0], args) == -1) {
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {

    pid_t pid = fork();

    if (pid == 0) {
        execute_command(argv[1], argv[2], argv[3]);
    } else {
        wait(NULL);
    }

    return 0;
}
