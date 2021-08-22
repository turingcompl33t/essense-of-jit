#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "io.h"
#include "error.h"
#include "subprocess.h"

char path[] = "/home/kad/dev/jit/fork/test/child";
char* argv[] = {NULL};

char message[] = "Hello World";

int main() {
  io_buffer_t buffer;
  io_buffer_init(&buffer);
  io_buffer_write(&buffer, (byte_t*)message, sizeof(message));

  int parent_to_child[2];
  if (-1 == pipe(parent_to_child)) fatal_error("pipe()");

  int child_to_parent[2];
  if (-1 == pipe(child_to_parent)) fatal_error("pipe()");

  // Run the child process
  subprocess_spawn(path, argv, parent_to_child[0], child_to_parent[1]);

  // Write the message to the pipe
  io_write_all(&buffer, parent_to_child[1]);
  io_buffer_clear(&buffer);

  // Read the response from the child
  io_read_all(&buffer, child_to_parent[0]);
  printf("%s\n", (char*)io_buffer_data(&buffer));

  io_buffer_clear(&buffer);
  return 0;
}
