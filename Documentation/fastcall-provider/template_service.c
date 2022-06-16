// SPDX-License-Identifier: GPL-2.0
/*
 * template service - Creates new directory and copies files from template directory
 * 
 * The following parameters are required:
 * @char *library_name	- name of the shared library.
 * @char *function_name	- name of the desired function.
 * @numer_of_arguments - number of arguments to pass to the function.
 * 
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define FILENAME_SIZE 2048
#define MAX_LINE 4096

void concatenation_service(char buffer[200], char *text)
{
	strcat(buffer, text);
};

int main(void)
{
	FILE *file, *temp, *binaryptr;
	char filename[FILENAME_SIZE] =
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_functions.S";
	char temp_filename[FILENAME_SIZE] =
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_functions.S";
	char buffer[MAX_LINE];
	char *library_path = "/home/atello/dev/hello-lib/copied_binary_new.txt";
	char *library_path_buffer = calloc(200, sizeof *library_path_buffer);
	char c;

	//line to add
	int write_line = 40;

	//Creates new directory within /drivers/
	system("mkdir /usr/local/src/linux/drivers/fastcall-template-example");

	//Copies files from /drivers/fastcall-template to new directory
	system("cp /usr/local/src/linux/drivers/fastcall-template/fastcall_driver.c \
    /usr/local/src/linux/drivers/fastcall-template/fastcall_functions.S \
    /usr/local/src/linux/drivers/fastcall-template/Makefile \
    /usr/local/src/linux/drivers/fastcall-template/Kconfig \
    /home/atello/dev/hello-lib/copied_binary_new.txt\
    /usr/local/src/linux/drivers/fastcall-template-example");

	//Copies desired function's binary from shared library
	concatenation_service(library_path_buffer, "cp ");
	concatenation_service(library_path_buffer, library_path);
	concatenation_service(library_path_buffer, " /usr/local/src/linux/drivers/fastcall-template-example");
	system(library_path_buffer);

	//Copies binary to buffer
	char *source = NULL;
	binaryptr = fopen(
		"/usr/local/src/linux/drivers/fastcall-template-example/copied_binary_new.txt",
		"r");
	if (binaryptr != NULL) {
		if (fseek(binaryptr, 0L, SEEK_END) == 0) {
			long bufsize = ftell(binaryptr);
			if (bufsize == -1) { /* Error */
			}
			/* Allocate our buffer to that size. */
			source = malloc(sizeof(char) * (bufsize + 1));

			/* Go back to the start of the file. */
			if (fseek(binaryptr, 0L, SEEK_SET) != 0) { /* Error */
			}

			/* Read the entire file into memory. */
			size_t newLen =
				fread(source, sizeof(char), bufsize, binaryptr);
			if (ferror(binaryptr) != 0) {
				fputs("Error reading file", stderr);
			} else {
				source[newLen++] = '\n'; /* Just to be safe. */
			}
		}
		fclose(binaryptr);
	}

	//Adds binary code segment to fastcall_functions.S
	//system("sed -i '82 i Tester' /usr/local/src/linux/drivers/fastcall-template-example/fastcall_functions.S");
	//strcpy(temp_filename, "temp____");
	//strcat(temp_filename, filename);

	file = fopen(filename, "r");
	temp = fopen(temp_filename, "w");
	bool keep_reading = true;
	int current_line = 1;
	do {
		// read the next line of the file into the buffer
		fgets(buffer, MAX_LINE, file);

		// if we've reached the end of the file, stop reading
		if (feof(file)) {
			keep_reading = false;
            fputs(buffer, temp);
		}
		// if we've reached the line to write the new line of text, write the line
		// of text to the temp file, followed by the line of text currently in the
		// original file at this line
		else if (current_line == write_line) {
			fputs(source, temp);
			fputs(buffer, temp);
		}
		// otherwise write this line to the temp file
		else {
			fputs(buffer, temp);
		}

		// increment the current line as we will now be reading the next line
		current_line++;

	} while (keep_reading);
	// close our access to both files as we are done with them
	fclose(file);
	fclose(temp);

	// delete the original file, rename temp file to the original file's name
	remove(filename);
	rename(temp_filename, filename);
	return 0;
}
