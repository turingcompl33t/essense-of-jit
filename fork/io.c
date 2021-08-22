/**
 * @file io.c
 * @brief Input / output utilities.
 */

#include "io.h"
#include "error.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE (64)

/**
 * Resize an IO buffer to accomadate request.
 * @param buffer The IO buffer
 * @param requested The number of bytes requested to be written
 */
static void io_buffer_resize(io_buffer_t* buffer, size_t requested) {
  // The new capacity must be AT LEAST this large
  const size_t new_min_capacity = buffer->size + requested;
  assert(new_min_capacity > buffer->capacity);

  size_t new_capacity = buffer->capacity > 0 ? buffer->capacity : 2;
  while (new_capacity < new_min_capacity) {
    new_capacity <<= 1;
  }

  buffer->buffer = realloc(buffer->buffer, new_capacity);
  buffer->capacity = new_capacity;
}

void io_buffer_init(io_buffer_t* buffer) {
  assert(buffer != NULL);
  buffer->buffer = NULL;
  buffer->size = 0;
  buffer->capacity = 0;
}

void io_buffer_clear(io_buffer_t* buffer) {
  assert(buffer != NULL);
  if (buffer->buffer != NULL) {
    free(buffer->buffer);
  }
  buffer->buffer = NULL;
  buffer->size = 0;
  buffer->capacity = 0;
}

byte_t* io_buffer_data(io_buffer_t* buffer) {
  assert(buffer != NULL);
  return buffer->buffer;
}

void io_buffer_read(io_buffer_t* buffer, byte_t* bytes, size_t count) {
  assert(buffer != NULL);
  size_t read_size = (count > buffer->size) ? buffer->size : count;
  memcpy(bytes, buffer->buffer, read_size);
}

void io_buffer_write(io_buffer_t* buffer, const byte_t* bytes, size_t count) {
  assert(buffer != NULL);
  if (buffer->size + count > buffer->capacity) {
    io_buffer_resize(buffer, count);
  }

  memcpy(buffer->buffer + buffer->size, bytes, count);
  buffer->size += count;
}

void io_read_all(io_buffer_t* buffer, int fd) {
  assert(buffer != NULL);
  
  ssize_t read_size;
  byte_t tmp[BUFFER_SIZE];
  while ((read_size = read(fd, tmp, sizeof(tmp))) > 0) {
    io_buffer_write(buffer, tmp, read_size);
    if (read_size < BUFFER_SIZE) break;
  }

  if (-1 == read_size) fatal_error("read()");
}

void io_write_all(io_buffer_t* buffer, int fd) {
  assert(buffer != NULL);
  size_t written = 0;
  byte_t* data = io_buffer_data(buffer);
  while (written < buffer->size) {
    ssize_t remaining = buffer->size - written;
    ssize_t write_size = write(fd, data + written, remaining);
    if (-1 == write_size) fatal_error("write()");
    written += write_size;
  }
}
