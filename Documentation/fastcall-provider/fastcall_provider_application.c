/*
 * User space application fastcall-provider
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "fastcall_provider_application_headers.h"

int main(void)
{
	int fd;
	int result = 42;
	struct ioctl_args args;

	//open fastcall-provider device
	fd = open(FCP_REGISTRATION_PATH, O_RDONLY); //@todo - check the flag
	if (fd < 0) {
		perror("open failed");
		return -1;
	}

	result = ioctl(fd, FCP_IOCTL_REGISTER_FASTCALL, &args);
	//register a new library so function
	if (result < 0) {
		perror("ioctl failed");
		return -1;
	} else {
		printf("%d\n", result);
		printf("%d\n", args.file_name);
	}

	if (close(fd)) {
		perror("close failed");
		return -1;
	}
	return 0;
}
