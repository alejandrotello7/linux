#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

char code[] = {0x55,0x48,0x89,0xe5,0x89,0x7d,0xec,0x89,
    0x89,0x75,0xf0,0xb8,0x2a,0x00,0x00,0x00,0xc9,0xc3,0x00};
/*

*/

int main(int argc, char **argv) { 
    void *buf;

    /* copy code to executable buffer */    
    buf = mmap (0,sizeof(code),PROT_READ|PROT_WRITE|PROT_EXEC,
                MAP_PRIVATE|MAP_ANON,-1,0);
    memcpy (buf, code, sizeof(code));
    __builtin___clear_cache(buf, buf+sizeof(code)-1);  // on x86 this just stops memcpy from optimizing away as a dead store

    /* run code */
    int i = ((int (*) (void))buf)();
    printf("get this done. returned: %d", i);

    return 0;
}