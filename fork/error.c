/**
 * @file error.c
 * @brief Error handling utilities.
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void fatal_error(const char* msg) {
  const int code = errno;
  printf("%s : %d\n", msg, errno);
  exit(code);
}
