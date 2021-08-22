#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "io.h"

int main() {
  io_buffer_t buffer;
  io_buffer_init(&buffer);

  io_read_all(&buffer, STDIN_FILENO);
  io_write_all(&buffer, STDOUT_FILENO);

  io_buffer_clear(&buffer);
  return 0;
}
