/**
 * @file driver2.c
 * @brief Calling non-JITed code from JITed code.
 */

#include <stdio.h>
#include <stdlib.h>

#include "jit.h"

// The signature of the function we will JIT
typedef int (*return_one_f)();

const char source[] = 
  "int id(int x);\n"
  "int return_one() { return id(1); }";

/** The function we call from JITed code */
int id(int x) {
  return x;
}

int main() {
  // Compile the source to a module
  module_t* module = jit_compile(source);

  // Resolve the function of interest
  return_one_f fn = jit_resolve_symbol(module, "return_one");

  // Call the JITed function
  int one = fn();
  printf("result: %d\n", one);

  jit_release(module);
  return EXIT_SUCCESS;
}
