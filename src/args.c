#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

void add_arg(char ***args, int *args_count, char *buffer, int *buffer_index) {
    *args = realloc(*args, (sizeof **args * ((*args_count) + 1)));
    (*args)[*args_count] = malloc(*buffer_index + 1);
    memcpy((*args)[*args_count], buffer, *buffer_index);
    (*args)[*args_count][*buffer_index] = '\0';
    *args_count = *args_count + 1;
    *buffer_index = 0;
}

void handle_char(char ***args, int *args_count, char *buffer, int *buffer_index, char current_char,
                 char end_char, int *in_char) {
    if (current_char != end_char) {
        buffer[*buffer_index] = current_char;
        *buffer_index = *buffer_index + 1;
    } else {
        add_arg(args, args_count, buffer, buffer_index);
        *in_char = FALSE;
    }
}

void fill_default_delimitators(struct delimitor **delimitators) {
    delimitators = malloc(sizeof(**delimitators) * 4);
    struct delimitor *del1 = malloc(sizeof *del1);
    del1->delim_char = ' ';
    del1->is_in = FALSE;
    struct delimitor *del2 = malloc(sizeof *del1);
    del2->delim_char = '\'';
    del2->is_in = FALSE;
    struct delimitor *del3 = malloc(sizeof *del1);
    del3->delim_char = '"';
    del3->is_in = FALSE;
    delimitators[0] = del1;
    delimitators[1] = del2;
    delimitators[2] = del3;
    delimitators[3] = NULL;
}

char **parse_args(const char *input) {
    struct delimitor **delimitors;
    fill_default_delimitators(delimitors);
    int in_quote = FALSE;
    int in_space = FALSE;
    int in_double_quote = FALSE;
    size_t input_len = strlen(input);
    int index = 0;
    char buffer[input_len];
    int buffer_index = 0;
    char **args = malloc(sizeof *args);
    int args_count = 0;
    while (index < input_len) {
        char current_char = input[index];
        if (in_double_quote == FALSE && current_char == '\'' && input_len > index + 1 &&
            input[index + 1] == '\'') {
            index++;
        } else if (in_quote == FALSE && current_char == '"' && input_len > index + 1 &&
                   input[index + 1] == '"') {
            index++;
        } else if (in_quote == FALSE && in_space == FALSE && in_double_quote == FALSE) {
            while (input[index] == ' ')
                index++;
            if (input[index] == '\'') {
                in_quote = TRUE;
            } else if (input[index] == '"') {
                in_double_quote = TRUE;
            } else {
                buffer[buffer_index++] = input[index];
                in_space = TRUE;
            }
        } else if (in_quote == TRUE) {
            handle_char(&args, &args_count, buffer, &buffer_index, current_char, '\'', &in_quote);
        } else if (in_space == TRUE) {
            handle_char(&args, &args_count, buffer, &buffer_index, current_char, ' ', &in_space);
        } else if (in_double_quote == TRUE) {
            handle_char(&args, &args_count, buffer, &buffer_index, current_char, '"', &in_double_quote);
        }
        index++;
    }
    if (buffer_index != 0) {
        add_arg(&args, &args_count, buffer, &buffer_index);
    }
    args = realloc(args, (sizeof *args * ((args_count) + 1)));
    args[args_count] = NULL;
    return args;
}

void free_args(char **args) {
    if (!args)
        return;
    int i = 0;
    while (args[i] != NULL)
        free(args[i++]);
    free(args);
}

void print_args(char **args) {
    int index = 0;
    while (args[index] != NULL) {
        printf("hallo %i\n", index);
        printf("args[%i]='%s'\n", index, args[index]);
        index++;
    }
}
