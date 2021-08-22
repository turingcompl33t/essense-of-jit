/**
 * @file driver.c
 * @brief JIT driver.
 */

#include <stdio.h>
#include <stdlib.h>

#include "jit.h"

// Some source code to JIT
const char source[] = "int return_one() { return 1; }";

// The signature for the function we JIT
typedef int (*add_one_f)();

int main() {
  
  // Compile the source to a module
  module_t* module = jit_compile(source);

  // Resolve the function of interest
  add_one_f fn = jit_resolve_symbol(module, "return_one");

  // Call the JITed function
  int one = fn();
  printf("result: %d\n", one);

  jit_release(module);
  return EXIT_SUCCESS;
}
