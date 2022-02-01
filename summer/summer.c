#include <linux/kernel.h>
		
#include <linux/syscalls.h>
		

		
SYSCALL_DEFINE2(summer,int, arg1, int, arg2){

	pr_warn("from summer kernel");
	printk("arg1: %d",arg1);
	int sum = arg1 + arg2;
	return sum;
		
}
