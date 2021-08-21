/**
 * @file jit.c
 * @brief Simple JIT library.
 */

#include "jit.h"
#include "error.h"

#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/mman.h>

// The signature for the function we will JIT
typedef int (*add_one_f)(int);

/** Determine the system page size */
long page_size() {
  long r = sysconf(_SC_PAGE_SIZE);
  if (-1 == r) fatal_error("sysconf()");
  return r;
}

void* jit_buffer_alloc(size_t size) {
  void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (MAP_FAILED == ptr) fatal_error("mmap()");
  return ptr;
}

void jit_buffer_free(void* ptr, size_t size) {
  if (munmap(ptr, size) == -1) {
    fatal_error("munmap()");
  }
}

void jit_buffer_finish(void* ptr, size_t size) {
  if (mprotect(ptr, size, PROT_READ | PROT_EXEC) == -1) {
    fatal_error("mprotect()");
  }
}

void emit_code(void* ptr, byte_t* code, size_t length) {
  memcpy(ptr, code, length);
}
