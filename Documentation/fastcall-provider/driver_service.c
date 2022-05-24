// SPDX-License-Identifier: GPL-2.0
/*
 * template_service - service for copying file
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

void add_line_service(int line_number, char filename[], char temp_filename[],
		      char newline[])
{
	FILE *driverptr, *temp;
	char buffer[MAX_LINE];

	driverptr = fopen(filename, "r");
	temp = fopen(temp_filename, "w");
	bool keep_reading = true;
	int current_line = 1;
	do {
		// read the next line of the file into the buffer
		fgets(buffer, MAX_LINE, driverptr);

		// if we've reached the end of the file, stop reading
		if (feof(driverptr)) {
			keep_reading = false;
			fputs(buffer, temp);
		}
		// if we've reached the line to write the new line of text, write the line
		// of text to the temp file, followed by the line of text currently in the
		// original file at this line
		else if (current_line == line_number) {
			fputs(newline, temp);
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
	fclose(driverptr);
	fclose(temp);

	// delete the original file, rename temp file to the original file's name
	remove(filename);
	rename(temp_filename, filename);
	return;
}
void replace_line_service(int line_number, char filename[],
			  char temp_filename[], char newline[])
{
	FILE *driverptr, *temp;
	char buffer[MAX_LINE];

	driverptr = fopen(filename, "r");
	temp = fopen(temp_filename, "w");
	bool keep_reading = true;
	int current_line = 1;
	do {
		// read the next line of the file into the buffer
		fgets(buffer, MAX_LINE, driverptr);

		// if we've reached the end of the file, stop reading
		if (feof(driverptr)) {
			keep_reading = false;
			printf("%i\n", current_line);
			//fputs(buffer, temp);
		}
		// if we've reached the line to write the new line of text, write the line
		// of text to the temp file
		else if (current_line == line_number) {
			fputs(newline, temp);
		}
		// otherwise write this line to the temp file
		else {
			fputs(buffer, temp);
		}

		// increment the current line as we will now be reading the next line
		current_line++;

	} while (keep_reading);
	// close our access to both files as we are done with them
	fclose(driverptr);
	fclose(temp);

	// delete the original file, rename temp file to the original file's name
	remove(filename);
	rename(temp_filename, filename);
	return;
}

int main(void)
{
	/* Adds function label to the driver */
	add_line_service(
		21,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_driver.c",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_driver.c",
		"void new_function(void);\n");

	/* Changes device name to the driver */
	replace_line_service(
		33,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_driver.c",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_driver.c",
		"#define DEVICE_NAME (\"fastcall-template/example1\")\n");

	/* Changes start function of the driver */
	replace_line_service(
		119,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_driver.c",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_driver.c",
		"\t\t.off = new_function - fct_functions_start,\n");

	/* Changes Makefile of the driver */
	replace_line_service(
		5,
		"/usr/local/src/linux/drivers/fastcall-template-example/Makefile",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____Makefile",
		"obj-$(CONFIG_FASTCALL_TEMPLATE_EXAMPLE)	+= fastcall_template_example.o\n");

	replace_line_service(
		9,
		"/usr/local/src/linux/drivers/fastcall-template-example/Makefile",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____Makefile",
		"fastcall_template_example-y	+= fastcall_driver.o fastcall_functions.o\n");

	/* Changes Kconfig file */
    replace_line_service(
		6,
		"/usr/local/src/linux/drivers/fastcall-template-example/Kconfig",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____Kconfig",
		"config FASTCALL_TEMPLATE_EXAMPLE\n");

	/* Adds argument definition to functions.S file */
	add_line_service(
		14,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_functions.S",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_functions.S",
		"#define ARG1 %rsi\n");	
	
	add_line_service(
		15,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_functions.S",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_functions.S",
		"#define ARG2 %rdx\n");			
			

	return 0;
}
