/**
 * @file io.h
 * @brief Input / output utilities.
 */

#ifndef IO_H
#define IO_H

#include <stdlib.h>

// IO routines operate on bytes
typedef unsigned char byte_t;

/**
 * The io_buffer type is a simple dynamic
 * buffer that manages its own capacity. =
 */
typedef struct io_buffer {
  /** The underlying buffer */
  byte_t* buffer;
  /** The current number of bytes in the buffer */
  size_t size;
  /** The current capacity of the buffer */
  size_t capacity;
} io_buffer_t;

/**
 * Initialize an IO buffer for use.
 * @param buffer The IO buffer
 */
void io_buffer_init(io_buffer_t* buffer);

/**
 * Free all resources associated with an IO buffer.
 * @param buffer The IO buffer
 */
void io_buffer_clear(io_buffer_t* buffer);

/**
 * Access the raw underlying buffer.
 * @param buffer The IO buffer
 */
byte_t* io_buffer_data(io_buffer_t* buffer);

/**
 * Read `count` bytes from the beginning of an IO
 * buffer into `bytes`.
 * @param buffer The buffer from which bytes are read
 * @param bytes The destination to which bytes are written
 * @param count The number of bytes to read
 */
void io_buffer_read(io_buffer_t* buffer, byte_t* bytes, size_t count);

/**
 * Write `count` bytes from `bytes` to an IO buffer.
 * @param buffer The buffer to which bytes are written
 * @param bytes The source from which bytes are written
 * @param count The number of bytes to write
 */
void io_buffer_write(io_buffer_t* buffer, const byte_t* bytes, size_t count);

/**
 * Read all available bytes from `fd` into an IO buffer.
 * @param buffer The buffer into which bytes are written
 * @param fd The descriptor from which bytes are read
 */
void io_read_all(io_buffer_t* buffer, int fd);

/**
 * Write all bytes in `buffer` to `fd`.
 * @param buffer The buffer from which bytes are read
 * @param fd The descriptor to which bytes are written
 */
void io_write_all(io_buffer_t* buffer, int fd);

#endif // IO_H
