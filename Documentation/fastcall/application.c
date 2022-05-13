// SPDX-License-Identifier: GPL-2.0
/*
 * application.c - example for calling fastcall function from an application
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SYS_FASTCALL (442)

// The ioctl definitions are the same as in the kernel code.
#define IOCTL_TYPE (0xDC)
//#define IOCTL_TEMPLATE (_IOR(IOCTL_TYPE, 0, struct ioctl_args))
//#define FCE_TYPE 0xDE
#define FCE_IOCTL(cmd) (_IOR(FCE_TYPE, cmd, struct ioctl_args))
#define FCE_IOCTL_NOOP (FCE_IOCTL(0))
#define FCE_IOCTL_STACK (FCE_IOCTL(1))
#define FCE_IOCTL_NONE _IO(FCE_TYPE, 6)
#define FCE_IOCTL_PRIV (FCE_IOCTL(2))
#define FCE_IOCTL_SUM (FCE_IOCTL(7))
#define FCE_IOCTL_SUMMER (FCE_IOCTL(8))
#define FCE_IOCTL_PROVIDER (FCE_IOCTL(9))
#define FCE_IOCTL_NOP_MACHINE (FCE_IOCTL(10))
#define IOCTL_TEMPLATE (_IOR(IOCTL_TYPE, 0, struct ioctl_args))


//#define DEVICE_PATH ("/dev/fastcall-examples")
#define DEVICE_PATH ("/dev/fastcall-template")

#define MAGIC (10)

struct ioctl_args {
	unsigned long fn_addr;
	unsigned long fn_len;
	//unsigned long shared_addr;
	unsigned index;
};

int main(void)
{
	struct ioctl_args args;
	int fd;

	// Open the device to control it via ioctl.
	fd = open(DEVICE_PATH, O_RDONLY);
	if (fd < 0) {
		perror("open failed");
		return 1;
	}

	// Register a new fastcall function.
	if (ioctl(fd, IOCTL_TEMPLATE, &args) < 0) {
		perror("ioctl failed");
		return 1;
	}

	// Use the shared memory area.
	//*((unsigned long *)args.shared_addr) = MAGIC;

	int returnValue = 0;
  printf("Address value: %lu\n", args.fn_addr);

	returnValue = syscall(SYS_FASTCALL, args.index,1,2);
	printf("Return value: %d\n", returnValue);

	// Perform the actual fastcall.
	/*if (returnValue != arg1+arg2) {
    fprintf(stderr, "syscall failed\n");
    return 1;
  }*/
	/*printf("Arg value 1: %d\n", arg1);
  printf("Arg value 2: %d\n", arg2);
  printf("Return value: %d\n", returnValue);  */

	// For deregistration unmap the fastcall function memory area.
	/*if (munmap((void *)args.fn_addr, args.fn_len)) {
    perror("munmap failed");
    return 1;
  }*/

	// We can also close the file descriptor.
	if (close(fd)) {
		perror("close failed");
		return 1;
	}

	return 0;
}
