/**
 * @file jit.h 
 * @brief Simple JIT library.
 */

#ifndef JIT_H
#define JIT_H

#include <stdlib.h>

/** For emitting code into the JIT buffer */
typedef unsigned char byte_t;

/** 
 * Allocate a page-aligned RW buffer.
 * @param size The size of the buffer
 * @return A pointer to the start of the buffer
 */
void* jit_buffer_alloc(size_t size);

/** 
 * Free a JIT buffer.
 * @param ptr A pointer to the buffer
 * @param size The size of the buffer
 */
void jit_buffer_free(void* ptr, size_t size);

/** 
 * Change buffer protections RW -> RX. 
 * @param ptr A pointer to the buffer
 * @param size The size of the buffer
 */
void jit_buffer_finish(void* ptr, size_t size);

/**
 * Emit code into an existing JIT buffer.
 * @param ptr A pointer to position at which bytes are appended
 * @param code A pointer to the code buffer
 * @param length The number of bytes in the code buffer
 */
void emit_code(void* ptr, byte_t* code, size_t length);

#endif // JIT_H
