#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int sz = 20 * 1024 * 1024; 
    char *p = malloc(sz);

    if (p == 0) {
        printf("stress: could not allocate memory\n");
        exit(1);
    }

    for (int i = 0; i < sz; i += 4096) {
        p[i] = 'x';
    }

    printf("Allocated 20 MB. Now run 'free' in another shell...\n");
    printf("Press Enter to free memory and exit.\n");
    
    char buf[1];
    read(0, buf, 1);

    free(p);
    exit(0);
}
