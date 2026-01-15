#include <nuttx/config.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#include "uart_comm.h"

int main(int argc, FAR char *argv[]) {
  int uart1_fd = open(RX_UART_DEVICE, O_RDWR);

  if (uart1_fd == -1) {
    printf("ERROR: Could not open usart1, %d\n", errno);
    return -1;
  }

  if (init_uart(uart1_fd, B115200, true) == -1) {
    printf("ERROR: Could not initialize usart1\n");
    return -1;
  }

  send_string(uart1_fd, "Hello from subbu world\r\n");

  return 0;
}

static int init_uart(int fd, speed_t baudrate, bool is_tx) {
  struct termios tio;

  if (tcgetattr(fd, &tio) != 0) {
    printf("ERROR: failed to get attribtues: %d\n", errno);
    return -1;
  }

  cfsetspeed(&tio, baudrate);

  // No need to configure these for USART1 since it is the NSH console.
  if (is_tx) {
    tio.c_cflag &= ~PARENB;
    tio.c_cflag &= ~CSTOPB;
    tio.c_cflag &= ~CSIZE;
    tio.c_cflag |= CS8;
    tio.c_cflag |= (CLOCAL | CREAD);
    // tio.c_cflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    // tio.c_iflag &= ~(IXON | IXOFF | IXANY);
    // tio.c_oflag &= ~OPOST;
  }

  if (tcsetattr(fd, TCSANOW, &tio) != 0) {
    printf("ERROR: failed to set attribtues: %d\n", errno);
    return -1;
  }

  return 0;
}

static void send_string(int fd, const char *str) {
  ssize_t bytes_written;
  size_t len = strlen(str);

  bytes_written = write(fd, str, len);
  if (bytes_written == -1) {
    printf("ERROR: Write failed, %d\n", errno);
  }
  else if (bytes_written != len) {
    printf("WARNING: Partial bytes written: %d/%d\n", bytes_written, len);
  }
  else {
    printf("Data sent to USART1\n");
  }
}
