#ifndef SHELL_H
#define SHELL_H

/* ANSI Color and Style Codes */
#define ANSI_RESET        "\033[0m"
#define ANSI_BOLD         "\033[1m"
#define ANSI_DIM          "\033[2m"
#define ANSI_UNDERLINE    "\033[4m"

#define ANSI_RED          "\033[31m"
#define ANSI_GREEN        "\033[32m"
#define ANSI_YELLOW       "\033[33m"
#define ANSI_BLUE         "\033[34m"
#define ANSI_MAGENTA      "\033[35m"
#define ANSI_CYAN         "\033[36m"
#define ANSI_WHITE        "\033[37m"

#define ANSI_BOLD_RED     "\033[1;31m"
#define ANSI_BOLD_GREEN   "\033[1;32m"
#define ANSI_BOLD_YELLOW  "\033[1;33m"
#define ANSI_BOLD_BLUE    "\033[1;34m"
#define ANSI_BOLD_MAGENTA "\033[1;35m"
#define ANSI_BOLD_CYAN    "\033[1;36m"
#define ANSI_BOLD_WHITE   "\033[1;37m"

int str_cmp(const char *str1, const char *str2);
void shell_readline(char *buffer, int max_length);
int parse_cmdline(char *buffer, char *argv[], int max_args);
int clear(int argc, char *argv[]);
int info(int argc, char *argv[]);
int help(int argc, char *argv[]);
int pong(int argc, char *argv[]);
int shell_execute(int argc, char *argv[]);
void shell_run();
typedef struct {
  char *name;
  char *description;
  int (*handler)(int argc, char *argv[]);
} command;

#endif
