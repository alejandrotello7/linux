#include <linux/kernel.h>
		
#include <linux/syscalls.h>
		

		
SYSCALL_DEFINE2(summer,unsigned int, arg1, unsigned int, arg2){

	int argument1 = 0;
	int argument2 = 0;
	pr_warn("from summer kernel");
	copy_from_user((void *)argument1, (void *)arg1, sizeof(unsigned int));
	copy_from_user((void *)argument2, (void *)arg2, sizeof(unsigned int));
	long sum = argument1 + argument2;
		
	return sum;
		
}
