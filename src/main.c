#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

void handle_cmd(char *cmd) {
  char *token = strtok(cmd, " ");
  // printf("token='%s'\n", token);
  if (strcmp(token, "exit") == 0) {
    token = strtok(NULL, " ");
    if (token[0] == '0')
      exit(EXIT_SUCCESS);
    else
      exit(EXIT_FAILURE);
  }

  printf("%s: command not found\n", cmd);
}

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while (1) {
    printf("$ ");
    size_t len = 0;
    char *line = NULL;
    ssize_t bytes_read = getline(&line, &len, stdin);
    line[bytes_read - 1] = '\0';
    handle_cmd(line);
  }

  return 0;
}
