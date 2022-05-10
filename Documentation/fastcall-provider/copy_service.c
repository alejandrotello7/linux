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
	FILE *fptr1, *fptr2;
	char c;
    const char *library_path = "/home/atello/dev/summer-lib/";
    const char *library_name = "libsummermodule.so";
	const char *function_name = "summer_module";
    const char *new_file = "temp_copied_binary.txt";
    char temp_buffer[200];
    char library_path_buffer[200];
    char temp_file_buffer[200];
    char new_file_buffer[200];

    strcat(strcpy(library_path_buffer, "cd "), library_path);
    strcat(library_path_buffer, "&& objdump -d ");
    strcat(library_path_buffer, library_name);
    strcat(library_path_buffer, " | awk -v RS= \'/^[[:xdigit:]]+ <");
    strcat(library_path_buffer, function_name);
    strcat(library_path_buffer, ">/\' | cut -f 2 | grep -v \':\' > ");
    strcat(library_path_buffer, new_file);
    printf("%s\n", library_path_buffer);
    system(library_path_buffer);

    strcat(strcpy(temp_file_buffer,library_path), "temp_copied_binary.txt");
    strcat(strcpy(new_file_buffer,library_path), "copied_binary.txt");

	fptr1 = fopen(temp_file_buffer, "r");
	fptr2 = fopen(new_file_buffer, "w");
	int x = 0;
	char temp[2];

	c = fgetc(fptr1);
	while (c != EOF) {
		if (c != ' ' && c != '\n') {
			temp[x] = c;
			x++;
		}
		if (x == 2) {
			fputs(".byte 0x", fptr2);
			fputc(temp[0], fptr2);
			fputc(temp[1], fptr2);
			fputs(";", fptr2);
			fputs("\n", fptr2);
			x = 0;
		}
		c = fgetc(fptr1);
	}

	return 0;
}
