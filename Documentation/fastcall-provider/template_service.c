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

int main(void)
{
	/* 
    Creates new directory within /drivers/ 
    */
	system("mkdir /usr/local/src/linux/drivers/fastcall-template-example-directory");

    /* 
     Copies files from /drivers/fastcall-template to new directory
    */
	system("cp /usr/local/src/linux/drivers/fastcall-template/fastcall_driver.c /usr/local/src/linux/drivers/fastcall-template/fastcall_functions.S .");

	return 0;
}
