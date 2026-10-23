#include "irq.h"
#include "mailbox.h"
#include "mini_uart.h"
#include "oled.h"
#include "peripherals/mailbox.h"
#include "pl011.h"
#include "pong.h"
#include "printf.h"
#include "shell.h"
#include "spi.h"
#include "timer.h"
#include "utils.h"
#include "video.h"

#define BORDER_MARGIN 40
void kernel_main(void) {
  uart_init();
  init_printf(0, pl011_putc);
  uart_send_string("Hello, world!\r\n");
  int el = get_el();
  printf("Exception level: %d\r\n", el);
  shell_run();

  while (1) {
    // uart_send(uart_recv());
  }
}
