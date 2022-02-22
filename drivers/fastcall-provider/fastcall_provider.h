//
// Created by atello on 10.01.22.
//

#ifndef LINUX_FASTCALL_PROVIDER_H
#define LINUX_FASTCALL_PROVIDER_H
#define FCP_DEVICE_NAME "fastcall-provider"
#define MAX_MINOR_DEVICES 100

#define FCP_TYPE 0xDA
#define FCP_IOCTL(cmd) (_IOR(FCP_TYPE, cmd, int))
#define FCP_IOCTL_REGISTER_FASTCALL (FCP_IOCTL(1))
#define FCP_IOCTL_REGISTER_FASTCALL_tester (FCP_IOCTL(2))
#define FCP_IOCTL_REGISTER_FUNCTION (FCP_IOCTL(3))

struct ioctl_args {
	unsigned int file_name;
	unsigned int code_size;
	char binary_code[128];
};

extern void print_fastcall(void); 

#endif //LINUX_FASTCALL_PROVIDER_H
