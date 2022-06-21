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
			if (current_line == line_number) {
				fputs(newline, temp);
			}
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
	//Counts how many lines are in the function's binary
	FILE *fptr1 = fopen("/usr/local/src/linux/drivers/fastcall-template-example/copied_binary_new.txt", "r");
	char ch;
	int number_of_lines = 0;
	while(!feof(fptr1))
	{
  		ch = fgetc(fptr1);
  		if(ch == '\n')
  		{
    		number_of_lines++;
  		}
	}
	//added 3 extra for the argument definition
	number_of_lines+=3;

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
		"#define DEVICE_NAME (\"fastcall-templates/example1\")\n");

	
	/* Change IOCTL number */
	replace_line_service(
		42,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_driver.c",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_driver.c",
		"#define IOCTL_TEMPLATE (_IOR(IOCTL_TYPE, 1, struct ioctl_args))\n");

	/* Changes start function of the driver */
	replace_line_service(
		20,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_driver.c",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_driver.c",
		"void fct_example_functions_start(void);\n");

	replace_line_service(
		23,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_driver.c",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_driver.c",
		"void fct_example_functions_end(void);\n");

	replace_line_service(
		28,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_driver.c",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_driver.c",
		"#define FUNCTION_SIZE ((unsigned long)(fct_example_functions_end - fct_example_functions_start))\n");

	replace_line_service(
		119,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_driver.c",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_driver.c",
		"\t\t.off = new_function - fct_example_functions_start,\n");
	
	replace_line_service(
		275,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_driver.c",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_driver.c",
		"memcpy(addr, fct_example_functions_start + page_id * PAGE_SIZE, count);\n");

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

	/* Changes internal labels for start and end of function.S file */
	replace_line_service(
		19,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_functions.S",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_functions.S",
		"SYM_INNER_LABEL(fct_example_functions_start, SYM_L_GLOBAL)\n");		

	replace_line_service(
		83+number_of_lines,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_functions.S",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_functions.S",
		"SYM_INNER_LABEL(fct_example_functions_end, SYM_L_GLOBAL)\n");		

	replace_line_service(
		40+number_of_lines,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_functions.S",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_functions.S",
		"SYM_CODE_START(fct_example_template)\n");	

	replace_line_service(
		81+number_of_lines,
		"/usr/local/src/linux/drivers/fastcall-template-example/fastcall_functions.S",
		"/usr/local/src/linux/drivers/fastcall-template-example/temp____fastcall_functions.S",
		"SYM_CODE_END(fct_example_template)\n");

	/* Adds new module to /linux/drivers Makefile */
	add_line_service(
		18,
		"/usr/local/src/linux/drivers/Makefile",
		"/usr/local/src/linux/drivers/temp____Makefile",
		"obj-y				+= fastcall-template-example/\n");

	/* Adds new module to /linux/drivers Kconfig */
	add_line_service(
		246,
		"/usr/local/src/linux/drivers/Kconfig",
		"/usr/local/src/linux/drivers/temp____Kconfig",
		"source \"drivers/fastcall-template-example/Kconfig\"\n");
	return 0;
}
