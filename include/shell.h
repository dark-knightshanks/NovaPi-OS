#ifndef SHELL_H
#define SHELL_H

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
