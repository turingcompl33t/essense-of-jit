/**
 * @file driver0.c
 * @brief A first attempt.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"

#define BUFFER_SIZE (256)

// For emitting code
typedef unsigned char byte_t;

// The signature of the function we will JIT
typedef int (*return_one_f)();

// The program we will JIT
byte_t code[] = {
  0xB8, 0x01, 0x00, 0x00, 0x00, // mov eax, 0x1
  0xC3                          // ret
};

/** Allocate a memory buffer for code */
void* jit_buffer_alloc(size_t size) {
  void* buffer = malloc(size);
  if (NULL == buffer) fatal_error("malloc()");
  return buffer;
}

/** Free a JIT memory buffer */
void jit_buffer_free(void* ptr) {
  free(ptr);
}

/** Emit code into the JIT buffer */
void emit_code(void* ptr, byte_t* code, size_t length) {

}

int main() {
  // Allocate a memory buffer for our code
  void* buffer = jit_buffer_alloc(BUFFER_SIZE);

  // Emit code into the buffer
  emit_code(buffer, code, sizeof(code));

  // Tell the compiler how to call our function
  return_one_f fn = buffer;

  // Call our function
  int one = fn();
  printf("result: %d\n", one);

  jit_buffer_free(buffer);
  return EXIT_SUCCESS;
}
