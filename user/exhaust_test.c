#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void open_files(int limit)
{
 
    printf("------------- Testing exhaustion. Limit is: %d ------------------\n", limit);

    int fd;
    int i = 3;
    for(; i < limit + 5; i++) {
        fd = open("testfile", O_CREATE | O_RDWR);
        if(fd < 0) 
        {
            printf("Stopped at count: %d (Cannot open more files)\n", i);
            break;
        }
        printf("Opened FD: %d\n", fd);
    }
    if(i-3 == limit) {
        printf("SUCCESS: System blocked opening files at the limit!\n");
    } else {
        printf("FAILED: System allowed opening %d files but limit was %d\n", i, limit);
    }
}

int main() {
    struct rlimit lim;
    
    //testing with normal system limit (16)
    //getrlimit(0, &lim);
    //int limit = lim.rlim_cur;
   // open_files(limit);
    
    //testing with modified limit (10)
    uint64 lim2=10;
    setrlimit(0,lim2);
    getrlimit(0, &lim);
    int limit = lim.rlim_cur;  
    open_files(limit);
    exit(0);
}

