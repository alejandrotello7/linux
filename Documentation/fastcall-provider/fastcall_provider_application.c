/*
 * User space application fastcall-provider
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "fastcall_provider_application_headers.h"

int main(void){
	int fd;

	//open fastcall-provider device
	fd = open(DEVICE_PATH); //@todo - check the flag
	if(fd < 0){
		perror("open failed");
		return -1;
	}

	//register a new library so function
	if(ioctl(fd, FCP_IOCTL_REGISTER_FASTCALL, FUNCTION_NAME) < 0 ){
		perror("ioctl failed");
		return -1;
	}

	if(close(fd)){
		perror("close failed");
		return -1;
	}
	return 0;
}

