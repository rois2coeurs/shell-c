#include "args.h"
#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 128
#define BUILT_IN_CMD                                                                               \
    { "echo", "type", "exit", "pwd", "cd" }
#define BUILD_IN_CMD_LEN 5

int is_in_path(char *cmd, char *res_path) {
    char *raw_paths = getenv("PATH");
    if (raw_paths == NULL)
        return 0;
    char paths[strlen(raw_paths) + 1];
    strcpy(paths, raw_paths);
    char *path = strtok(paths, ":");
    while (path != NULL) {
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
                if (stat(res_buffer, &fs) < 0)
                    continue;
                if (!S_ISREG(fs.st_mode))
                    continue;
                if (!(fs.st_mode & S_IXUSR))
                    continue;
                strcpy(res_path, res_buffer);
                return 1;
            }
        }
        path = strtok(NULL, ":");
    }
    return 0;
}

void exit_cmd(char *exit_status) {
    if (exit_status == NULL || exit_status[0] == '0')
        exit(EXIT_SUCCESS);
    else
        exit(EXIT_FAILURE);
}

void echo_cmd(char *args[]) {
    int index = 0;
    while (args[index] != NULL) {
        printf("%s", args[index]);
        index++;
        if (args[index] != NULL)
            printf(" ");
    }
    printf("\n");
}

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

void pwd_cmd() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    }
}

void cd_cmd(char *path) {
    if (strcmp(path, "~") == 0)
        path = getenv("HOME");
    if (chdir(path) == 0)
        return;
    printf("cd: %s: No such file or directory\n", path);
}

void execute_external_file(char *exe_path, char **args) {
    pid_t pid = fork();
    if (pid == 0) {
        execv(exe_path, args);
        exit(127);
    } else {
        waitpid(pid, 0, 0);
    }
}

void handle_cmd(char *cmd, int cmd_len) {
    char cmd_cpy[cmd_len];
    memcpy(cmd_cpy, cmd, cmd_len);
    char path_buffer[1024];
    char **args = parse_args(cmd);

    if (strcmp(args[0], "exit") == 0) {
        exit_cmd(args[1]);
    } else if (strcmp(args[0], "echo") == 0) {
        echo_cmd(args + 1);
    } else if (strcmp(args[0], "type") == 0) {
        type_cmd(args[1]);
    } else if (strcmp(args[0], "pwd") == 0) {
        pwd_cmd();
    } else if (strcmp(args[0], "cd") == 0) {
        cd_cmd(args[1]);
    } else if (is_in_path(args[0], path_buffer)) {
        execute_external_file(path_buffer, args);
    } else {
        printf("%s: command not found\n", cmd);
    }
    free_args(args);
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
