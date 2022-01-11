/*
 * fastcall_provider_driver.c - driver for fastcall provider application at
 * user level.
 */
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/minmax.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/fastcall.h>
#include <asm/pgtable.h>
#include <asm/cpufeature.h>
#include "fastcall_provider.h"

MODULE_DESCRIPTION(
	"Driver to Fast Call provider user service");
static dev_t fcp_dev;

//Global storage for device Major number
static int dev_major = 0;

/*
 * fcp_ioctl() - register ioctl handlers
 */
static long fcp_ioctl(struct file *file, unsigned int cmd, unsigned long args)
{
	//If there is no ioctl command
	long ret = -ENOIOCTLCMD;

	switch(cmd){
	case FCP_IOCTL_REGISTER_FASTCALL:
		//todo
		ret = 0;
		break;
	}
	return ret == 1 ? -EFAULT : ret;

}

/*
 * fcp_init() - initialize fastcall-provider module
 * Adds fastcall-provider character device.
 */
static int __init fcp_init(void)
{
	int result;
	static struct file_operations fops = {
		.owner = THIS_MODULE,
		.unlocked_ioctl = fcp_ioctl,
	};

	//Allocate character device number
	result = alloc_chrdev_region(&fcp_dev, 0, MAX_MINOR_DEVICES, FCE_DEVICE_NAME);
	if(result < 0){
		pr_warn("fcp: Can't allocate chrdev region")
		return -1;
	}

	//Save device major number in global variable
	dev_major = MAJOR(dev);

	//Allocate character device struct
	fcp_cdev = cdev_alloc();
	if(fcp_cdev == NULL){
		pr_warn("fcp: Can't allocate struc cdev");
		result = -ENOMEM;
		unregister_chrdev_region(fcp_dev, MAX_MINOR_DEVICES);
		return result;
	}
	fcp_cdev->owner = THIS_MODULE;
	fcp_cdev->ops = &fops;

	//Add character device to the kernel
	result = cdev_add(fcp_cdev, fcp_dev, MAX_MINOR_DEVICES);
	if(result < 0 ){
		pr_warn("fcp: can't add character device");
		cdev_del(fcp_cdev);
		return result;
	}

	//Create a class for the device
	fcp_class = class_create(THIS_MODULE, FCP_DEVICE_NAME);
	if(IS_ERR_VALUE(fcp_class)){
		pr_warn("fcp: can't create class");
	}

	//Create fastcall provider device and link it in /dev/ directory
	fcp_device = device_create(fcp_class, NULL, fcp_dev, NULL, FCP_DEVICE_NAME);
	if (IS_ERR_VALUE(fcp_device)) {
		pr_warn("fcp: can't create device");
		result = PTR_ERR(fcp_device);
		class_destroy(fcp_class);
		return -1;
	}
	return result;
}

static void __exit fcp_exit(void)
{
	device_destroy(fcp_class, fcp_dev);
	class_destroy(fcp_class);
	cdev_del(fcp_cdev);
	unregister_chrdev_region(fcp_dev, MAX_MINOR_DEVICES);
}

module_init(fcp_init);
module_exit(fcp_exit);

MODULE_LICENSE("GPL");

