#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    struct meminfo mi;
    
    if(meminfo(&mi) < 0){
        printf("Error: meminfo syscall failed\n");
        exit(1);
    }

    printf("--- System Memory Info ---\n");
    printf("Free Memory: %ld bytes\n", mi.free_mem);
    printf("Used Memory: %ld bytes\n", mi.used_mem);
    printf("Total System Memory: %ld bytes\n", mi.free_mem + mi.used_mem);
    
    exit(0);
}
