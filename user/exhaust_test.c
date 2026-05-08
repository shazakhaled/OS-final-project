#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main() {
    struct rlimit lim;
    getrlimit(0, &lim);
    int limit = lim.rlim_cur;
    
    printf("------------- Testing exhaustion. Limit is: %d ------------------\n", limit);

    int fd;
    int count = 0;
    for(int i = 3; i < limit + 5; i++) {
        fd = open("testfile", O_CREATE | O_RDWR);
        if(fd < 0) {
            printf("Stopped at count: %d (Cannot open more files)\n", i);
            break;
        }
        printf("Opened FD: %d\n", fd);
        count = i;
    }

    if(count == limit) {
        printf("SUCCESS: System blocked opening files at the limit!\n");
    } else {
        printf("FAILED: System allowed opening %d files but limit was %d\n", count + 1, limit);
    }

    exit(0);
}

