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
#include "copy_service.h"

extern void copy_service(void);

int main(void)
{
    copy_service();
	return 0;
}
