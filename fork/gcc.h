/**
 * @file gcc.c
 * @brief Wrapper for spawning GCC to compile source code.
 */

#ifndef GCC_H
#define GCC_H

#include <stdlib.h>

/**
 * Compile C source code to a shared object.
 * @param source The source code
 * @return The absolute path to the shared object
 * 
 * NOTE: User is responsible for:
 * - freeing the string allocated for the path
 * - unlinking the shared object file
 */
char* gcc_compile(const char* source);

#endif // GCC_H
