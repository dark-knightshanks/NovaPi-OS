#include "shell.h"
#include "pl011.h"
#include "pong.h"
#include "printf.h"
#include "utils.h"

static const command commands[] = {{"help", "List available commands", help},
                                   {"info", "Show system info", info},
                                   {"clear", "Clear Terminal screen", clear},
                                   {"pong", "Play a pong game", pong},
                                   {0, 0, 0}};

int str_cmp(const char *str1, const char *str2) {
  while (*str1 != '\0' && (*str1 == *str2)) {
    str1++;
    str2++;
  }
  return (*str1 - *str2);
}

void shell_readline(char *buffer, int max_length) {
  int index = 0;
  while (1) {
    char c = uart_recv();
    if (c == '\r' || c == '\n') {
      buffer[index] = '\0';
      uart_send('\r');
      uart_send('\n');
      return;
    } else if (c == '\b' || c == 127) {
      if (index > 0) {
        index = index - 1;
        uart_send('\b');
        uart_send(' ');
        uart_send('\b');
      }
    } else if (c >= 32 && c <= 126) {
      if (index < (max_length - 1)) {
        buffer[index] = c;
        index = index + 1;
        uart_send(c);
      }
    }
  }
}

int parse_cmdline(char *buffer, char *argv[], int max_args) {
  int argc = 0;
  char *ptr = buffer;

  while (*ptr != '\0' && argc < max_args) {
    while (*ptr == ' ' || *ptr == '\t') {
      ptr++;
    }
    if (*ptr == '\0') {
      break;
    }

    argv[argc] = ptr;
    argc++;

    while (*ptr != '\0' && *ptr != ' ' && *ptr != '\t') {
      ptr++;
    }

    if (*ptr != '\0') {
      *ptr = '\0';
      ptr++;
    }
  }
  return argc;
}

int clear(int argc, char *argv[]) {
  printf("\033[2J\033[H"); // ANSI code for clearing Terminal
  return 0;
}

int info(int argc, char *argv[]) {
  printf("NovaPi OS (AArch64)\r\n");
  printf("Exception Level: %d\r\n", get_el());
  return 0;
}

int help(int argc, char *argv[]) {
  printf("Available commands are:\r\n");
  for (int i = 0; commands[i].name != 0; ++i) {
    printf("  %s - %s\r\n", commands[i].name, commands[i].description);
  }
  return 0;
}

int pong(int argc, char *argv[]) {
  printf("Launching pong game ... \n");
  start_pong_game();
  return 0;
}

int shell_execute(int argc, char *argv[]) {
  if (argc == 0) {
    return 0;
  }
  for (int i = 0; commands[i].name != 0; ++i) {
    if (str_cmp(argv[0], commands[i].name) == 0) {
      commands[i].handler(argc, argv);
      return 0;
    }
  }
  printf("Command not found: %s\r\n", argv[0]);
  return 1;
}

void shell_run() {
  char line_buff[128];
  char *argv[16];
  printf("\r\n=== Welcome to NovaPi Shell ===\r\n");
  while (1) {
    printf("novapi>> ");
    shell_readline(line_buff, sizeof(line_buff));
    int argc = parse_cmdline(line_buff, argv, 16);
    shell_execute(argc, argv);
  }
}
