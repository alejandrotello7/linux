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
	FILE *fptr1, *fptr2;
	char c;
	fptr1 = fopen("./test.txt", "r");
	fptr2 = fopen("./new.txt", "w");
	do {
		fputs(".byte ", fptr2);
		c = fgetc(fptr1);
		fputc(c, fptr2);
		c = fgetc(fptr1);
		fputc(c, fptr2);
		fputs(";", fptr2);
	} while (c != EOF);

	return 0;
}
