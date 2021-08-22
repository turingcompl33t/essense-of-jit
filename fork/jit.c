/**
 * @file jit.c
 * @brief Simple JIT via fork()/exec().
 */

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "gcc.h"
#include "error.h"
#include "loader.h"

/**
 * A module contains all of the metadata necessary
 * to resolve and execute JIT-compiled functions.
 */
typedef struct module {
  /** The absolute path to the object file */
  const char* object_path;
  /** The handle to the loaded shared library */
  void* handle;
} module_t;

module_t* jit_compile(const char* source) {
  module_t* module = malloc(sizeof(module_t));
  if (NULL == module) fatal_error("malloc()");

  // Compile the source to a shared object
  const char* object_path = gcc_compile(source);
  
  // Open a handle to the library
  void* handle = loader_load(object_path);

  module->object_path = object_path;
  module->handle = handle;
  return module;
}

void jit_release(module_t* module) {
  assert(module != NULL);

  // Unload the shared object
  loader_unload(module->handle);

  // Remove the source file
  unlink(module->object_path);
  free((void*)(module->object_path));

  free(module);
}

void* jit_resolve_symbol(module_t* module, const char* symbol) {
  assert(module != NULL);
  // TODO: Cache resolved symbols
  return loader_resolve(module->handle, symbol);
}
