#define FCP_TYPE 0xDA
#define FCP_IOCTL(cmd) (_IOR(FCP_TYPE, cmd, int))
#define FCP_IOCTL_REGISTER_FASTCALL (FCP_IOCTL(1))
#define FCP_IOCTL_REGISTER_FASTCALL_tester (FCP_IOCTL(2))

#define DEVICE_PATH ("/dev/fastcall-provider/fp0")
#define LIBRARY_PATH ("/path/to/library")

struct ioctl_args {
	unsigned int file_name;
};

//char const *FUNCTION_NAME = "ucp_worker_close_ifaces\n";
