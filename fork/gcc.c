/**
 * @file gcc.c
 * @brief Wrapper for spawning GCC to compile source code.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>

#include "io.h"
#include "error.h"
#include "subprocess.h"

#define INPUT_EXT_LENGTH     (2)  // .c
#define OUTPUT_EXT_LENGTH    (3)  // .so
#define BUFFER_SIZE        (128)

/**
 * NOTE: There were a couple of things I wanted to do in 
 * this implementation that I ran into trouble with: 
 * - First, I wanted to avoid creation of a temporary file
 * 	 for the input and just get GCC to read from its stdin
 * 	 over a pipe. This is fine, and it reads the source, 
 *   but after that it continues to hang on reading from
 * 	 the pipe (debugged with strace) even when I close the
 * 	 write end in the parent process. I'm sure this is
 * 	 just something small I am overlooking, but man it is
 * 	 frustrating trying to debug these issues.
 * - Once I resolved to just read the input from a temporary
 * 	 file, I ran into the second issue of writing the output
 * 	 file (the shared object). Naturally, GCC allows one to
 * 	 specify the path to the output with -o, but for some 
 * 	 reason I always got a linker error ("no such file")
 * 	 when using this feature. Eventually I just let GCC write
 * 	 to its default location in the current directory and
 * 	 then mv the output file where I want it. 
 */

/** 
 * Create the path to a temporary file to use as
 * the input to GCC.
 * @return The path to the file
 * 
 * NOTE: Caller is reponsible for free()
 */
static char* gcc_make_input_filename() {
	char buffer[L_tmpnam + INPUT_EXT_LENGTH];
	memset(buffer, 0, sizeof(buffer));

	tmpnam(buffer);
	size_t length = strlen(buffer);

	char* result = malloc(length  + INPUT_EXT_LENGTH + 1);
	sprintf(result, "%s.c", buffer);
	result[length + INPUT_EXT_LENGTH] = '\0';

	return result;
}

/**
 * Create the path to a temporary file to use as
 * the output from GCC.
 * @return The path to the file
 * 
 * NOTE: Caller is responsible for free()
 */
static char* gcc_make_output_filename() {
	char buffer[L_tmpnam + OUTPUT_EXT_LENGTH];
	memset(buffer, 0, sizeof(buffer));
	
	// Get a temporary filename
	// NOTE: tmpnam is unsafe
	tmpnam(buffer);
	size_t length = strlen(buffer);

	// Append the shared object extension
	char* result = malloc(length + OUTPUT_EXT_LENGTH + 1);
	sprintf(result, "%s.so", buffer);
	result[length + OUTPUT_EXT_LENGTH] = '\0';

	return result;
}

/**
 * Build a shared object from the given source code.
 * @param source The source code
 * @return `true` if the operation is successful, `false` otherwise
 */
static bool gcc_build_shared_object(const char* source) {
	// Write the source code to a temporary file
	char* input_path = gcc_make_input_filename();
	FILE* input = fopen(input_path, "w");
	if (NULL == input) fatal_error("fopen()");

	// The source code is a null-terminated string,
	// but there is no need to write the terminator
	// to the input source file for GCC
	const size_t write_size = strlen(source);
	if (fwrite(source, sizeof(byte_t), write_size, input) < write_size) {
		fatal_error("fwrite()");
	}
	fclose(input);


	// gcc -fPIC -shared -xc <input_path> 
	// Construct the vector of arguments for GCC
	char* argv[6] = {
		"gcc",       // convention
		"-fPIC",     // build position-independent code
		"-shared",   // compiled a shared library
		"-xc",       // tell GCC the type of source
		input_path,
		NULL
	};

	// Spawn the subprocess
	int ret = subprocess_wait(
		subprocess_spawn("/usr/bin/gcc", argv, -1, -1));

	// The source file is no longer needed
	unlink(input_path);
	free(input_path);

	return (0 == ret);
}

/**
 * Move the shared object from the current directory
 * to a safer location in the /tmp directory.
 * @return The path to where the file is moved
 */
static char* gcc_move_shared_object() {
	char directory[BUFFER_SIZE];
	memset(directory, 0, sizeof(directory));
	if (getcwd(directory, sizeof(directory)) == NULL) fatal_error("getcwd");

	char input_path[BUFFER_SIZE*2];
	memset(input_path, 0, sizeof(input_path));
	sprintf(input_path, "%s/%s", directory, "a.out");

	char* output_path = gcc_make_output_filename();

	// Construct arguments: mv input_path output_path
	char* argv[4] = {
		"mv",
		input_path,
		output_path,
		NULL
	};

	int ret = subprocess_wait(
		subprocess_spawn("/usr/bin/mv", argv, -1, -1));
	if (ret != 0) {
		free(output_path);
		fatal_error("mv");
	}

	return output_path;
}

char* gcc_compile(const char* source) {
	if (gcc_build_shared_object(source)) {
		return gcc_move_shared_object();
	}
	fatal_error("failed to compile shared object");
}
