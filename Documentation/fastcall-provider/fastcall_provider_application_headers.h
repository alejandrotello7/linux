#define FCP_TYPE 0xDA
#define FCP_IOCTL(cmd) (_IOR(FCP_TYPE, cmd, int))
#define FCP_IOCTL_REGISTER_FASTCALL (FCP_IOCTL(1))
#define FCP_IOCTL_REGISTER_FASTCALL_tester (FCP_IOCTL(2))
#define FCP_IOCTL_REGISTER_FUNCTION (FCP_IOCTL(3))


#define FCP_REGISTRATION_PATH ("/dev/fastcall-provider/fp0")
#define LIBRARY_PATH ("/path/to/library")

struct ioctl_args {
	unsigned int file_name;
	unsigned int code_size;
	char binary_code[128];
};

