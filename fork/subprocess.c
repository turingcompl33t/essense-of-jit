/**
 * @file subprocess.c
 * @brief Subprocess management.
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "error.h"

pid_t subprocess_spawn(const char* pathname, char* argv[], int in_fd, int out_fd) {
  pid_t pid = fork();
  if (-1 == pid) fatal_error("fork()");

  if (0 == pid) {
    // Child process
    if (in_fd != -1) {
      // Redirect standard input from in_fd
      dup2(in_fd, STDIN_FILENO);
    }

    if (out_fd != -1) {
      // Redirect standard output, standard error to out_fd
      dup2(out_fd, STDOUT_FILENO);
      dup2(out_fd, STDERR_FILENO);
    }

    if (-1 == execv(pathname, argv)) fatal_error("execv()");
  }

  // Parent process
  return pid;
}

int subprocess_wait(pid_t pid) {
  int status;
  if (-1 == waitpid(pid, &status, 0)) fatal_error("waitpid()");
  if (WIFEXITED(status)) {
    return WEXITSTATUS(status);
  }
  fatal_error("subprocess exited abnormally");
}
