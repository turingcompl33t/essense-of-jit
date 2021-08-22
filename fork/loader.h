/**
 * @file loader.h
 * @brief Dynamic loading utilities.
 */

#ifndef LOADER_H
#define LOADER_H

/**
 * Load the dynamic library identified by `filename`.
 * @param filename The path to the library
 * @return A handle to the library
 */
void* loader_load(const char* filename);

/**
 * Close the library identified by `handle`.
 * @param handle The handle for the library
 */
void loader_unload(void* handle);

/**
 * Resolve a symbol in a dynamic library.
 * @param handle The handle to the library
 * @param symbol The symbol to resolve
 * @return The address to which the symbol is resolved
 */
void* loader_resolve(void* handle, const char* symbol);

#endif // LOADER_H
