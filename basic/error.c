/**
 * @file error.c
 * @brief Error handling utilities.
 */

#include "error.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void fatal_error(const char* msg) {
  const int code = errno;
  printf("%s : %d\n", msg, code);
  exit(code);
}
