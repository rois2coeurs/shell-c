#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handle_cmd(char *cmd) {
  char *token = strtok(cmd, " ");
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
  char buffer[128];

  while (1) {
    printf("$ ");
    scanf("%[^\n]s", buffer);
    handle_cmd(buffer);
    memset(buffer, '\0', 128);
  }

  return 0;
}
