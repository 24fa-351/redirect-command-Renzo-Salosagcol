#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute_command(char *input, char *command, char *output) {
    int input_descriptor, output_descriptor, pipe_fd[2];

    pipe(pipe_fd);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        if (strcmp(inp, "-") != 0) {
            inp_fd = open(inp, O_RDONLY);
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

        close(pipe_fd[0]); // Close unused read end
        dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipe_fd[1]);

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
    } else { // Parent process
        close(pipe_fd[1]); // Close unused write end
        wait(NULL);
        close(pipe_fd[0]);
    }
}

int main(int argc, char *argv[]) {

    execute_command(argv[1], argv[2], argv[3]);

    return 0;
}
