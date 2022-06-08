// SPDX-License-Identifier: GPL-2.0
/*
 * copy_service - Copies the binary from the desired library function
 * 
 * The following parameters are required:
 * @char *library_path 	- absolute path to shared library location within the system.
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
#include <errno.h>

int main(void)
{
	FILE *fptr1, *fptr2, *fptr3;
	char c;
	const char *library_path = "/home/atello/dev/summer-lib/";
	const char *library_name = "libsummermodule.so";
	const char *function_name = "summer_module";
	const int number_of_arguments = 2;
	const char *new_file = "temp_copied_binary.txt";
	char temp_buffer[200];
	char library_path_buffer[200];
	char temp_file_buffer[200];
	char new_file_buffer[200];
	char new_file_buffer_copy[200];

	strcat(strcpy(library_path_buffer, "cd "), library_path);
	strcat(library_path_buffer, " && objdump -d ");
	strcat(library_path_buffer, library_name);
	strcat(library_path_buffer, " | awk -v RS= \'/^[[:xdigit:]]+ <");
	strcat(library_path_buffer, function_name);
	strcat(library_path_buffer, ">/\' | cut -f 2 | grep -v \':\' > ");
	strcat(library_path_buffer, new_file);
	//printf("%s\n", library_path_buffer);

	if (system(library_path_buffer) != 0) {
		printf("Error while reading the function from the shared library %d\n",
		       errno);
		goto fail_reading;
	}

	strcat(strcpy(temp_file_buffer, library_path),
	       "temp_copied_binary.txt");
	strcat(strcpy(new_file_buffer, library_path), "copied_binary.txt");
	strcat(strcpy(new_file_buffer_copy, library_path),
	       "copied_binary_new.txt");

	fptr1 = fopen(temp_file_buffer, "r");
	fptr2 = fopen(new_file_buffer, "w");
	if (!fptr1 || !fptr2) {
		fclose(fptr1);
		fclose(fptr2);
		printf("Error while opening the files %d\n", errno);
		goto fail_open_file;
	}
	int x = 0;
	char temp[2];

	c = fgetc(fptr1);
	while (c != EOF) {
		if (c != ' ' && c != '\n') {
			fputc(c, fptr2);
		}
		c = fgetc(fptr1);
	}
	fclose(fptr1);
	fclose(fptr2);

	fptr2 = fopen(new_file_buffer, "r+");
	if (!fptr2) {
		goto fail_open_file;
	}

	//Removes the first 2 instructions from the .SO file
	strcat(strcpy(temp_buffer, "cd "), library_path);
	strcat(temp_buffer, " && sed -i -e 's/554889e5//g' copied_binary.txt");
	if (system(temp_buffer) != 0) {
		printf("Error while removing lines from the shared library %d\n",
		       errno);
		fclose(fptr2);
		goto fail_reading;
	}

	//Removes the last 2 instructions from the .SO file
	strcat(strcpy(temp_buffer, "cd "), library_path);
	strcat(temp_buffer, " && sed -i -e 's/5dc3//g' copied_binary.txt");
	if (system(temp_buffer) != 0) {
		printf("Error while removing lines from the shared library %d\n",
		       errno);
		fclose(fptr2);
		goto fail_reading;
	}

	fclose(fptr2);

	fptr2 = fopen(new_file_buffer, "r");
	fptr3 = fopen(new_file_buffer_copy, "w");

	if (!fptr2 || !fptr3) {
		fclose(fptr2);
		fclose(fptr3);
		printf("Error while opening the files %d\n", errno);
		goto fail_open_file;
	}

	fputs("/* \n * New fastcall function \n */ \n", fptr3);

	switch (number_of_arguments) {
	case 0:
		fputs("SYM_CODE_START(new_function)\nUNWIND_HINT_EMPTY \nFASTCALL_SETUP_STACK \npushq %rbp \npushq %rdi \nmovq %rsp, %rbp\n",
		      fptr3);
		break;
	case 1:
		fputs("SYM_CODE_START(new_function)\nUNWIND_HINT_EMPTY \nFASTCALL_SETUP_STACK \npushq %rbp \npushq %rdi \nmovq %rsp, %rbp\nmovq ARG1, %rdi \n",
		      fptr3);
		break;
	case 2:
		fputs("SYM_CODE_START(new_function)\nUNWIND_HINT_EMPTY \nFASTCALL_SETUP_STACK \npushq %rbp \npushq %rdi \nmovq %rsp, %rbp\nmovq ARG1, %rdi \nmovq ARG2, %rsi\n",
		      fptr3);
		break;
	default:
		printf("Ivalid number of arguments\n");
		goto fail_number_of_arguments;
	}

	c = fgetc(fptr2);
	while (c != EOF) {
		if (c != ' ' && c != '\n') {
			temp[x] = c;
			x++;
		}
		if (x == 2) {
			fputs(".byte 0x", fptr3);
			fputc(temp[0], fptr3);
			fputc(temp[1], fptr3);
			fputs(";", fptr3);
			fputs("\n", fptr3);
			x = 0;
		}
		c = fgetc(fptr2);
	}

	fputs("popq %rdi \npopq %rbp \nmovq %rdi, %rsp \nsysretq\nSYM_CODE_END(new_function)\n",
	      fptr3);
	fclose(fptr2);
	fclose(fptr3);

fail_number_of_arguments:
fail_open_file:
fail_reading:
	return 0;
}
