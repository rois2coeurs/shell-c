#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  char buffer[128];

  while(1) {
      printf("$ ");
      scanf("%s", buffer);
      printf("%s: command not found\n", buffer);
  }

  return 0;
}
