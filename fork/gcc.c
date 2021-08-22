/**
 * @file gcc.c
 * @brief Wrapper for spawning GCC to compile source code.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>

#include "io.h"
#include "error.h"
#include "subprocess.h"

#define EXT_LENGTH        (3)
#define CMD_BUFFER_SIZE (256)

static char* gcc_make_filename() {
	char buffer[L_tmpnam + EXT_LENGTH];
	
	// Get a temporary filename
	// NOTE: tmpnam is unsafe
	tmpnam(buffer);

	// Append the shared object extension
	char* result = malloc(strlen(buffer) + EXT_LENGTH);
	sprintf(result, "%s.so", buffer);
	return result;
}

char* gcc_compile_shared_object(const char* source, size_t length) {
	char* path = gcc_make_filename();

	// gcc -fPIC -shared -x c -o <path> -

	// Construct the vector of arguments for GCC
	char output_dst[CMD_BUFFER_SIZE];
	memset(output_dst, 0, sizeof(output_dst));
	sprintf(output_dst, "-o %s", path);
	char* argv[7] = {
		"gcc",       // convention
		"-fPIC",     // build position-independent code
		"-shared",   // compiled a shared library
		"-xc",       // tell GCC the type of source
		output_dst,  // tell GCC where to write the result
		"-",         // read source from standard input
		NULL
	};

	int pipes[2];
	if (-1 == pipe(pipes)) fatal_error("pipe()");
	if (-1 == fcntl(pipes[0], F_SETFD, __O_CLOEXEC)) fatal_error("fcntl()");
	if (-1 == fcntl(pipes[1], F_SETFD, __O_CLOEXEC)) fatal_error("fcntl()");

	// Spawn the subprocess
	pid_t pid = subprocess_spawn("/usr/bin/gcc", argv, pipes[0], -1);

	// Write the source code to the pipe
	io_buffer_t buffer;
	io_buffer_init(&buffer);
	io_buffer_write(&buffer, (byte_t*)source, length);
	io_write_all(&buffer, pipes[1]);
	
	close(pipes[0]);
	close(pipes[1]);

	// Wait for GCC to exit
	int ret = subprocess_wait(pid);
	printf("ret = %d\n", ret);

	io_buffer_clear(&buffer);
	return path;
}
