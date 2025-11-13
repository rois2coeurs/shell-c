#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

void exit_cmd(char *exit_status) {
  if (exit_status[0] == '0')
    exit(EXIT_SUCCESS);
  else
    exit(EXIT_FAILURE);
}

void echo_cmd(char *message) { printf("%s\n", message); }

void handle_cmd(char *cmd, int cmd_len) {
  char cmd_cpy[cmd_len];
  memcpy(cmd_cpy, cmd, cmd_len);
  char *token = strtok(cmd_cpy, " ");
  // printf("token='%s'\n", token);
  if (strcmp(token, "exit") == 0) {
    token = strtok(NULL, " ");
    exit_cmd(token);
    return;
  }
  if (strcmp(token, "echo") == 0) {
    echo_cmd(cmd + 5);
    return;
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
    handle_cmd(line, bytes_read);
  }

  return 0;
}
