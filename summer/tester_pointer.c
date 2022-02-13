#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>

char code[] = {0x55,0x48,0x89,0xe5,0x89,0x7d,0xec,0x89,
		0x75, 0xe8, 0x8b, 0x55, 0xec, 0x8b, 0x45, 0xe8, 0x01,
		0xd0, 0x89, 0x45, 0xfc, 0x8b, 0x45, 0xfc, 0x5d, 0xc3 };
/*
    mov $10, %esi
    mov $2 , %edi
*/

int main(int argc, char **argv) { 
    void *buf;

    /* copy code to executable buffer */    
    buf = mmap (0,sizeof(code),PROT_READ|PROT_WRITE|PROT_EXEC,
                MAP_PRIVATE|MAP_ANON,-1,0);
    memcpy (buf, code, sizeof(code));
    __builtin___clear_cache(buf, buf+sizeof(code)-1);  // on x86 this just stops memcpy from optimizing away as a dead store


    __asm__("mov $10, %esi;"
                "mov $2, %edi;"
                );
    /* run code */
    int i = ((int (*) (void))buf)();
    printf("Result returned: %d\n", i);

    return 0;
}




 
