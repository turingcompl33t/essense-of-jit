/**
 * @file jit.h
 * @brief Simple JIT via fork()/exec().
 */

#ifndef JIT_H
#define JIT_H

typedef struct module module_t;

/**
 * JIT-compile source code to a module.
 * @param source The source code to compile
 * @return The module for the source code
 */
module_t* jit_compile(const char* source);

/**
 * Release all of the resources associated with `module`.
 * @param module The module to release
 */
void jit_release(module_t* module);

/**
 * Resolve a symbol in a JITed module.
 * @param module The module
 * @param symbol The symbol to resolve
 * @return The resolved symbol 
 */
void* jit_resolve_symbol(module_t* module, const char* symbol);

#endif // JIT_H
