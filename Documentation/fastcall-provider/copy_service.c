// SPDX-License-Identifier: GPL-2.0
/*
 * copy_service - example for copying file
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void)
{
	FILE *fptr1, *fptr2, *fptr3;
	char c;
	fptr1 = fopen("./test.txt", "r");
	fptr2 = fopen("./temp.txt", "w");
	fptr3 = fopen("./new.txt", "w");

	c = fgetc(fptr1);
	while (c != EOF) {
		if (c != ' ') {
			fputc(c, fptr2);
		}
		c = fgetc(fptr1);
	}

   	c = fgetc(fptr2);
    while (c != EOF)
    {
		fputs(".byte ", fptr3);
        c = fgetc(fptr2);
		fputc(c, fptr3);
		c = fgetc(fptr2);
		fputc(c, fptr3);
		fputs(";", fptr3);
	}

	return 0;
}
