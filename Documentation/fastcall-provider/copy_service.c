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
    const char *library_path = "~/dev/summer-lib/";
    const char *library_name = "libsummermodule.so";
	const char *function_name = "summer_module";
    const char *new_file = "test.txt";
    char temp_buffer[200];
    char library_path_buffer[100];
    char new_file_buffer[100];


    strcat(strcpy(temp_buffer, "objdump -d "), library_name);
    strcat(temp_buffer, " | awk -v RS= \'/^[[:xdigit:]]+ <");
    strcat(temp_buffer, function_name);
    strcat(temp_buffer, ">/\' | cut -f 2 | grep -v \':\' > ");
    strcat(temp_buffer, new_file);
    printf("%s\n", temp_buffer);
    system(temp_buffer);
    strcat(strcpy(library_path_buffer,library_path), "test.txt");
    strcat(strcpy(new_file_buffer,library_path), "new.txt");

	fptr1 = fopen(library_path_buffer, "r");
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
