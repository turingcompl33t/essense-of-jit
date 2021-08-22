/**
 * @file error.h
 * @brief Error handling utilities.
 */

#ifndef ERROR_H
#define ERROR_H

/**
 * Print an error message and terminate the program.
 * @param msg The message to print
 */
void fatal_error(const char* msg) __attribute__((noreturn));

#endif // ERROR_H
