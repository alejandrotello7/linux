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

MODULE_DESCRIPTION("Driver to Fast Call provider user service");

static dev_t fcp_dev;
static struct cdev *fcp_cdev;
static struct class *fcp_class;
static struct device *fcp_device;
static struct cdev *fcp_cdev_app;

//Global storage for device Major number
static int dev_major = 0;
static int counter = 2;
static atomic_t counter_atomic = ATOMIC_INIT(2);

/*
* add_application_device() - Creates new device node. Returns 0 on success, -1 on failure.
*/
static long add_application_device(unsigned long args)
{
	long result = 0;
	struct ioctl_args *io_args;

	if (IS_ERR_VALUE(fcp_class)) {
		pr_warn("fcp: can't create class");
		result = -1;
	}
	fcp_device = device_create(
		fcp_class, NULL, MKDEV(dev_major, atomic_read(&counter_atomic)),
		NULL, "fastcall-provider/fp%d", atomic_read(&counter_atomic));

	if (IS_ERR_VALUE(fcp_device)) {
		pr_warn("fcp_app: can't create device");
		pr_warn("fcp_app: last atomic read values was %d",
			atomic_read(&counter_atomic));
		result = PTR_ERR(fcp_device);
		goto fail_device_creation;
	}
	io_args = kzalloc(sizeof(struct ioctl_args), GFP_KERNEL);
	io_args->file_name = atomic_read(&counter_atomic);
	if (copy_to_user((void *)args, io_args, sizeof(struct ioctl_args)))
		goto fail_copy;
	counter++;
	atomic_inc(&counter_atomic);
	return 0;

fail_copy:
	kfree(io_args);
fail_device_creation:
	class_destroy(fcp_class);
	return result;
}

/*
* register_function() - copies the binary function from userspace
*/
static long register_function(unsigned long args)
{
	struct ioctl_args *iop_args;
	long result = 0;
	iop_args = kmalloc(sizeof(struct ioctl_args), GFP_KERNEL);
	if (copy_from_user(iop_args, (void *)args, sizeof(struct ioctl_args)))
		goto fail_copy;

	printk(KERN_INFO "fcp: value: %x", iop_args->binary_code[2]);
	//return iop_args->binary_code[2];
	return 20;
fail_copy:
	result = 42;
	kfree(iop_args);
	return result;
}

/*
 * fcp_ioctl() - register ioctl handlers
 */
static long fcp_ioctl(struct file *file, unsigned int cmd, unsigned long args)
{
	//If there is no ioctl command
	long ret = -ENOIOCTLCMD;

	switch (cmd) {
	case FCP_IOCTL_REGISTER_FASTCALL:
		ret = add_application_device(args);
		break;
	case FCP_IOCTL_REGISTER_FUNCTION:
		ret = register_function(args);
		break;
	default:
		pr_warn("fcp: invalid ioctl call");
	}

	return ret == -1 ? -EINVAL : ret;
}

/*
 * fcp_ioctl_app() - register ioctl handlers for app
 */
static long fcp_ioctl_app(struct file *file, unsigned int cmd,
			  unsigned long args)
{
	//If there is no ioctl command
	long ret = -ENOIOCTLCMD;

	switch (cmd) {
	case FCP_IOCTL_REGISTER_FASTCALL_tester:
		ret = 0;
		break;
	default:
		pr_warn("fcp: invalid ioctl call");
	}

	return ret == -1 ? -EINVAL : ret;
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

	static struct file_operations fops_app = {
		.owner = THIS_MODULE,
		.unlocked_ioctl = fcp_ioctl_app,
	};

	//Allocate character device number
	result = alloc_chrdev_region(&fcp_dev, 0, MAX_MINOR_DEVICES,
				     FCP_DEVICE_NAME);
	if (result < 0) {
		pr_warn("fcp: Can't allocate chrdev region");
		return -1;
	}

	//Save device major number in global variable
	dev_major = MAJOR(fcp_dev);

	//Allocate character device struct
	fcp_cdev = cdev_alloc();
	if (fcp_cdev == NULL) {
		pr_warn("fcp: Can't allocate struc cdev");
		result = -ENOMEM;
		unregister_chrdev_region(fcp_dev, MAX_MINOR_DEVICES);
		return result;
	}
	fcp_cdev->owner = THIS_MODULE;
	fcp_cdev->ops = &fops;

	//Allocate character device structure for application
	fcp_cdev_app = cdev_alloc();
	if (fcp_cdev_app == NULL) {
		pr_warn("fcp: Can't allocate struc cdev");
		result = -ENOMEM;
		unregister_chrdev_region(fcp_dev, MAX_MINOR_DEVICES);
		return result;
	}
	fcp_cdev_app->owner = THIS_MODULE;
	fcp_cdev_app->ops = &fops_app;

	//Add character device to the kernel
	result = cdev_add(fcp_cdev, MKDEV(dev_major, 0), MAX_MINOR_DEVICES);
	if (result < 0) {
		pr_warn("fcp: can't add character device");
		cdev_del(fcp_cdev);
		return result;
	}

	cdev_add(fcp_cdev_app, MKDEV(dev_major, 1), MAX_MINOR_DEVICES);

	//Create a class for the device
	fcp_class = class_create(THIS_MODULE, FCP_DEVICE_NAME);
	if (IS_ERR_VALUE(fcp_class)) {
		pr_warn("fcp: can't create class");
	}

	//Create fastcall provider device and link it in /dev/ directory
	fcp_device = device_create(fcp_class, NULL, MKDEV(dev_major, 0), NULL,
				   "fastcall-provider/fp%d", 0);
	if (IS_ERR_VALUE(fcp_device)) {
		pr_warn("fcp_init: can't create device");
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
