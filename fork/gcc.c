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
 * @param length The length of the source code
 * @return `true` if the operation is successful, `false` otherwise
 */
static bool gcc_build_shared_object(const char* source, size_t length) {
	// Write the source code to a temporary file
	char* input_path = gcc_make_input_filename();
	FILE* input = fopen(input_path, "w");
	if (NULL == input) fatal_error("fopen()");

	// The source code is a null-terminated string,
	// but there is no need to write the terminator
	// to the input source file for GCC
	size_t write_size = length - 1;
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

char* gcc_compile(const char* source, size_t length) {
	if (gcc_build_shared_object(source, length)) {
		return gcc_move_shared_object();
	}
	fatal_error("failed to compile shared object");
}
