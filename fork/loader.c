/**
 * @file loader.c
 * @brief Dynamic loading utilities.
 */

#include "error.h"
#include "loader.h"

#include <dlfcn.h>
#include <stdlib.h>

void* loader_load(const char* filename) {
  // TODO: Explore lazy resolution
  void* handle = dlopen(filename, RTLD_NOW);
  if (NULL == handle) fatal_error(dlerror());
  return handle;
}

void loader_unload(void* handle) {
  if (dlclose(handle) != 0) fatal_error(dlerror());
}

void* loader_resolve(void* handle, const char* symbol) {
  void* resolved = dlsym(handle, symbol);
  // This is not REALLY a fatal error because this
  // just indicates the symbol is not present, but
  // for this toy example we will just assume that
  // we will only resolve symbols we expect to see
  if (NULL == resolved) fatal_error(dlerror());
  return resolved;
}
