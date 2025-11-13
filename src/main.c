#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFER_SIZE 128
#define BUILT_IN_CMD                                                           \
  { "echo", "type", "exit" }
#define BUILD_IN_CMD_LEN 3

int is_in_path(char *cmd, char *res_path) {
  char *raw_paths = getenv("PATH");
  if (raw_paths == NULL)
    return 0;
  char paths[strlen(raw_paths) + 1];
  strcpy(paths, raw_paths);
  // printf("paths='%s'\n", paths);
  char *path = strtok(paths, ":");
  while (path != NULL) {
    // printf("path='%s'\n", path);
    struct dirent *de;
    DIR *dr = opendir(path);
    if (dr != NULL) {
      while ((de = readdir(dr)) != NULL) {
        if (strcmp(de->d_name, cmd) != 0)
          continue;
        char res_buffer[strlen(path) + strlen(de->d_name) + 2];
        strcpy(res_buffer, path);
        strcat(res_buffer, "/");
        strcat(res_buffer, de->d_name);
        struct stat fs;
        if (stat(res_buffer, &fs) < 0) continue;
        if (!S_ISREG(fs.st_mode)) continue;
        if (!(fs.st_mode & S_IXUSR)) continue;
        strcpy(res_path, res_buffer);
        return 1;
      }
    }
    path = strtok(NULL, ":");
  }
  return 0;
}

void exit_cmd(char *exit_status) {
  if (exit_status[0] == '0')
    exit(EXIT_SUCCESS);
  else
    exit(EXIT_FAILURE);
}

void echo_cmd(char *message) { printf("%s\n", message); }

void type_cmd(char *cmd) {
  char *arr[] = BUILT_IN_CMD;
  for (int i = 0; i < BUILD_IN_CMD_LEN; i++) {
    if (strcmp(cmd, arr[i]) == 0) {
      printf("%s is a shell builtin\n", arr[i]);
      return;
    }
  }
  char buffer[1024];
  if (is_in_path(cmd, buffer)) {
    printf("%s is %s\n", cmd, buffer);
    return;
  }
  printf("%s: not found\n", cmd);
}

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
  if (strcmp(token, "type") == 0) {
    type_cmd(cmd + 5);
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
