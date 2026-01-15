#ifndef __APPS_EXAMPLES_UART_COMM_H
#define __APPS_EXAMPLES_UART_COMM_H

#include <termios.h>
#include <stdbool.h>

#define TX_UART_DEVICE "/dev/ttyS2"   // USART3
#define RX_UART_DEVICE "/dev/ttyS0"   // USART0 -> also /dev/console
#define BUFFER_SIZE 252

static int init_uart(int, speed_t, bool);
static void send_string(int, const char *);

#endif
