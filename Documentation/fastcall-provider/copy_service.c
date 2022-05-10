// SPDX-License-Identifier: GPL-2.0
/*
 * copy_service - example for copying file
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	FILE *fptr1, *fptr2, *fptr3;
	char c;
	const char *library_path = "/home/atello/dev/summer-lib/";
	const char *library_name = "libsummermodule.so";
	const char *function_name = "summer_module";
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
	printf("%s\n", library_path_buffer);
	system(library_path_buffer);

	strcat(strcpy(temp_file_buffer, library_path),
	       "temp_copied_binary.txt");
	strcat(strcpy(new_file_buffer, library_path), "copied_binary.txt");
	strcat(strcpy(new_file_buffer_copy, library_path),
	       "copied_binary_new.txt");

	fptr1 = fopen(temp_file_buffer, "r");
	fptr2 = fopen(new_file_buffer, "w");
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
	system("cd /home/atello/dev/summer-lib/ && sed -i -e 's/554889e5//g' copied_binary.txt");
	system("cd /home/atello/dev/summer-lib/ && sed -i -e 's/5dc3//g' copied_binary.txt ");
	fclose(fptr2);

	fptr2 = fopen(new_file_buffer, "r");
	fptr3 = fopen(new_file_buffer_copy, "w");

	fputs("UNWIND_HINT_EMPTY \nFASTCALL_SETUP_STACK \npushq %rbp \npushq %rdi \nmovq %rsp, %rbp\nmovq ARG1, %rdi \nmovq ARG2, %rsi",
	      fptr3);
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

	fputs("popq %rdi \npopq %rbp \nmovq %rdi, %rsp \nsysretq", fptr3);
	fclose(fptr2);
	fclose(fptr3);

	return 0;
}
