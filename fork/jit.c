/**
 * @file jit.c
 * @brief Simple JIT via fork()/exec().
 */

#include <stdlib.h>

/**
 * A module contains all of the metadata necessary
 * to resolve and execute JIT-compiled functions.
 */
typedef struct module {
  /** The absolute path to the object file */
  const char* object;
  /** The handle to the loaded shared library */
  void* handle;
} module_t;

module_t* jit_compile(const char* source) {
  module_t* module = malloc(sizeof(module_t));
  return module;
}

void jit_release(module_t* module) {
  free(module);
}

void* jit_resolve_symbol(module_t* module, const char* symbol) {
  return NULL;
}
