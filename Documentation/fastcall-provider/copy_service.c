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
    c = fgetc(fptr1);
    while (c != EOF)
    {
        fputc(c, fptr2);
        fputs(".byte", fptr2);
        c = fgetc(fptr1);
    }
	return 0;
}
