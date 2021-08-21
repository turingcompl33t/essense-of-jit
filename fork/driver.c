/**
 * @file driver.c
 * @brief JIT driver.
 */

#include <stdio.h>
#include <stdlib.h>

#include "loader.h"

const char path[] = "/home/kad/dev/jit/fork/libtest.so";

typedef int (*add_one_f)();

int main() {
  void* handle = loader_load(path);
  add_one_f fn = loader_resolve(handle, "return_one");

  int one = fn();
  printf("result: %d\n", one);

  loader_unload(handle);
  return EXIT_SUCCESS;
}
