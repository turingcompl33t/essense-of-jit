/**
 * @file driver2.c
 * @brief A JIT example with input arguments.
 */

#include <stdio.h>
#include <stdlib.h>

#include "jit.h"

#define BUFFER_SIZE (256)

// The signature for the function we will JIT
typedef int (*add_f)(int, int);

// The program we will JIT
byte_t code[] = {
  0x48, 0x89, 0xF8,   // mov rax, rdi
  0x48, 0x01, 0xF0,   // add rax, rsi
  0xC3                // ret
};

int main() {
  // Allocate a writeable buffer
  void* buffer = jit_buffer_alloc(BUFFER_SIZE);

  // Write code to the buffer
  emit_code(buffer, code, sizeof(code));

  // Make the buffer executable
  jit_buffer_finish(buffer, BUFFER_SIZE);

  // Tell the compiler how to call our function
  add_f fn = buffer;

  // Call our function
  int three = fn(1, 2);
  printf("result: %d\n", three);

  jit_buffer_free(buffer, BUFFER_SIZE);
  return EXIT_SUCCESS;
}
