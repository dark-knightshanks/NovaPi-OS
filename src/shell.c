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
  printf(ANSI_BOLD_CYAN "NovaPi OS (AArch64)\r\n" ANSI_RESET);
  printf("Exception Level: " ANSI_BOLD_YELLOW "%d\r\n" ANSI_RESET, get_el());
  return 0;
}

int help(int argc, char *argv[]) {
  printf(ANSI_BOLD_GREEN "Available commands are:\r\n" ANSI_RESET);
  for (int i = 0; commands[i].name != 0; ++i) {
    printf("  " ANSI_BOLD_YELLOW "%s" ANSI_RESET " - %s\r\n", commands[i].name, commands[i].description);
  }
  return 0;
}

int pong(int argc, char *argv[]) {
  printf(ANSI_BOLD_MAGENTA "Launching pong game ... \r\n" ANSI_RESET);
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
  printf(ANSI_BOLD_RED "Command not found: %s\r\n" ANSI_RESET, argv[0]);
  return 1;
}

void shell_run() {
  char line_buff[128];
  char *argv[16];

  printf(ANSI_BOLD_CYAN "\r\n"
         "  _   _                 ____  _ \r\n"
         " | \\ | | _____   ____ _|  _ \\(_)  \r\n"
         " |  \\| |/ _ \\ \\ / / _` | |_) | | \r\n"
         " | |\\  | (_) \\ V / (_| |  __/| | \r\n"
         " |_| \\_|\\___/ \\_/ \\__,_|_|   |_| \r\n"
         ANSI_RESET);
  printf(ANSI_BOLD_GREEN "  AArch64 Bare-Metal OS for Raspberry Pi\r\n" ANSI_RESET);
  printf(ANSI_DIM "  Type 'help' to view available commands.\r\n\r\n" ANSI_RESET);

  while (1) {
    printf(ANSI_BOLD_CYAN "novapi>> " ANSI_RESET);
    shell_readline(line_buff, sizeof(line_buff));
    int argc = parse_cmdline(line_buff, argv, 16);
    shell_execute(argc, argv);
  }
}
