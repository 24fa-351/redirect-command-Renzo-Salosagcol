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

  if (pid == 0) { // Child process
    if (strcmp(input, "-") != 0) {
        input_descriptor = open(input, O_RDONLY);
        dup2(input_descriptor, STDIN_FILENO);
        close(input_descriptor);
    }
    if (strcmp(output, "-") != 0) {
        output_descriptor = open(output, O_WRONLY, 0644);
        dup2(output_descriptor, STDOUT_FILENO);
        close(output_descriptor);
    }

    close(pipe_fd[0]);
    dup2(pipe_fd[1], STDOUT_FILENO);
    close(pipe_fd[1]);

    char *args[20];
    char *token = strtok(command, " ");
    int index = 0;
    while (token != NULL) {
        args[index++] = token;
        token = strtok(NULL, " ");
    }
    args[index] = NULL;

  } else { 
    // Parent
    close(pipe_fd[1]); // Close unused write end
    wait(NULL);
    close(pipe_fd[0]);
  }
}

int main(int argc, char *argv[]) {

  execute_command(argv[1], argv[2], argv[3]);

  return 0;
}
