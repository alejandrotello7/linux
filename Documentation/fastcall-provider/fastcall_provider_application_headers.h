#define FCP_TYPE 0xDA
#define FCP_IOCTL(cmd) (_IOR(FCP_TYPE, cmd, int))
#define FCP_IOCTL_REGISTER_FASTCALL (FCP_IOCTL(1))

#define DEVICE_PATH ("/dev/fastcall-provider/0")
#define LIBRARY_PATH ("/path/to/library")

//char const *FUNCTION_NAME = "ucp_worker_close_ifaces\n";


