/**
 * @file driver1.c
 * @brief A working JIT example.
 */

#include <stdio.h>
#include <stdlib.h>

#include "jit.h"

#define BUFFER_SIZE (256)

// The signature for the function we will JIT
typedef int (*return_one_f)();

// The program we will JIT
byte_t code[] = {
  0xB8, 0x01, 0x00, 0x00, 0x00, // mov eax, 0x1
  0xC3                          // ret
};

int main() {
  // Allocate a writeable buffer
  void* buffer = jit_buffer_alloc(BUFFER_SIZE);

  // Write code to the buffer
  emit_code(buffer, code, sizeof(code));

  // Make the buffer executable
  jit_buffer_finish(buffer, BUFFER_SIZE);

  // Tell the compiler how to call our function
  return_one_f fn = buffer;

  // Call our function
  int one = fn();
  printf("result: %d\n", one);

  jit_buffer_free(buffer, BUFFER_SIZE);
  return EXIT_SUCCESS;
}
