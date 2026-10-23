#ifndef SHELL_H
#define SHELL_H

int str_cmp(const char *str1, const char *str2);
void shell_readline(char *buffer, int max_length);
int parse_cmdline(char *buffer, char *argv[], int max_args);
#endif

