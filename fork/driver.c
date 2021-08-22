/**
 * @file driver.c
 * @brief JIT driver.
 */

#include <stdio.h>
#include <stdlib.h>

// #include "loader.h"
#include "gcc.h"

// const char path[] = "/home/kad/dev/jit/fork/libtest.so";

// typedef int (*add_one_f)();

const char source[] = "int return_one() { return 1; }";

int main() {
  // void* handle = loader_load(path);
  // add_one_f fn = loader_resolve(handle, "return_one");

  // int one = fn();
  // printf("result: %d\n", one);

  // loader_unload(handle);
  
  char* name = gcc_compile_shared_object(source, sizeof(source));
  puts(name);
  free(name);

  return EXIT_SUCCESS;
}
