#ifndef H_ARGS
#define H_ARGS

char **parse_args(const char *input);
void free_args(char **args);
void print_args(char **args);

struct delimitor {
    char delim_char;
    int is_in;
};

#endif
