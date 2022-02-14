/*
 * User space application fastcall-provider
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "fastcall_provider_application_headers.h"


int main(void)
{
	int fd;
	int result = 42;
	struct ioctl_args *args;
	char code[] = {0x55,0x48,0x89,0xe5,0x89,0x7d,0xec,0x89,
		0x75, 0xe8, 0x8b, 0x55, 0xec, 0x8b, 0x45, 0xe8, 0x01,
		0xd0, 0x89, 0x45, 0xfc, 0x8b, 0x45, 0xfc, 0x5d, 0xc3 };

	args = (struct ioctl_args *) malloc(sizeof(struct ioctl_args)+sizeof(code));
	strcpy(args->binary_code,code);
	//printf("Value: %x", args->binary_code[0]);


	//open fastcall-provider device
	fd = open(FCP_REGISTRATION_PATH, O_RDONLY); //@todo - check the flag
	if (fd < 0) {
		perror("open failed");
		return -1;
	}

	/*result = ioctl(fd, FCP_IOCTL_REGISTER_FASTCALL, &args);
	//register a new library so function
	if (result < 0) {
		perror("ioctl failed");
		return -1;
	} else {
		printf("Return value: %d\n", result);
		printf("Newly device node was created under /dev/fastcall-provider/fp%d\n",
		       args.file_name);
	}*/

	result = ioctl(fd, FCP_IOCTL_REGISTER_FUNCTION, &args);
	if (result < 0) {
		perror("ioctl failed");
		return -1;
	} else {
		printf("Return value: %x\n", result);
	}




	if (close(fd)) {
		perror("close failed");
		return -1;
	}
	return 0;
}
