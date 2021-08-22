/**
 * @file subprocess.h
 * @brief Subprocess management.
 */

#ifndef SUBPROCESS_H
#define SUBPROCESS_H

#include <sys/types.h>

/**
 * Spawn a subprocess without waiting for it to complete.
 * @param pathname The path to the program
 * @param argv The arguments vector for the program
 * @param in_fd The descriptor used as standard input for 
 * the spawned subprocess, ignored if -1
 * @param out_fd The descriptor used as standard output for
 * the spawned subprocess, ignored if -1
 */
pid_t subprocess_spawn(const char* pathname, char* argv[], int in_fd, int out_fd);

/**
 * Wait for a spawned subprocess to terminate.
 * @param child_pid The PID of the subprocess
 */
int subprocess_wait(pid_t pid);

#endif // SUBPROCESS_H
