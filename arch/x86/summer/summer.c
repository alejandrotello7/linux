#include <linux/kernel.h>
		
#include <linux/syscalls.h>
		

		
SYSCALL_DEFINE2(summer,long *, arg1, long *, arg2){
		
	long sum = *arg1 + *arg2;
		
	return sum;
		
}
