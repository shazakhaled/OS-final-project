#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void cleanup(int fds[], int n)
{
    for(int i = 0; i < n; i++) {
        if(fds[i] >= 0)
            close(fds[i]);
    }
}

void test_normal_limit()
{
    printf("\n========== TEST 1: NORMAL LIMIT ==========\n");

    struct rlimit lim;

    setrlimit(0, 10);

    getrlimit(0, &lim);

    int limit = lim.rlim_cur;

    printf("Current limit = %d\n", limit);

    int fds[20];

    int i;
    for(i = 0; i < limit; i++) {

        fds[i] = open("testfile", O_CREATE | O_RDWR);

        if(fds[i] < 0) {
            printf("FAILED: Could not open file before limit\n");
            cleanup(fds, i);
            return;
        }

        printf("Opened FD: %d\n", fds[i]);
    }

    int fd = open("testfile", O_CREATE | O_RDWR);

    if(fd < 0)
        printf("SUCCESS: System blocked opening beyond limit\n");
    else {
        printf("FAILED: System allowed opening beyond limit\n");
        close(fd);
    }

    cleanup(fds, limit);
}

void test_zero_limit()
{
    printf("\n========== TEST 2: ZERO LIMIT ==========\n");

    setrlimit(0, 3);

    int fd = open("testfile", O_CREATE | O_RDWR);

    if(fd < 0)
        printf("SUCCESS: Open blocked correctly when limit reached\n");
    else {
        printf("FAILED: Open succeeded unexpectedly\n");
        close(fd);
    }
}

void test_close_reuse()
{
    printf("\n========== TEST 3: CLOSE AND REUSE ==========\n");

    setrlimit(0, 5);

    int fds[2];

    for(int i = 0; i < 2; i++) {

        fds[i] = open("testfile", O_CREATE | O_RDWR);

        if(fds[i] < 0) {
            printf("FAILED: Could not open initial files\n");
            cleanup(fds, i);
            return;
        }

        printf("Opened FD: %d\n", fds[i]);
    }

    int fd = open("testfile", O_CREATE | O_RDWR);

    if(fd < 0)
        printf("SUCCESS: Extra open blocked\n");
    else {
        printf("FAILED: Extra open succeeded\n");
        close(fd);
    }

    close(fds[0]);

    printf("Closed FD: %d\n", fds[0]);

    fd = open("testfile", O_CREATE | O_RDWR);

    if(fd >= 0) {
        printf("SUCCESS: Open succeeded after close\n");
        close(fd);
    }
    else
        printf("FAILED: Open still blocked after close\n");

    close(fds[1]);
}

void test_invalid_resource()
{
    printf("\n========== TEST 4: INVALID RESOURCE ==========\n");

    int result = setrlimit(99, 10);

    if(result < 0)
        printf("SUCCESS: Invalid resource rejected\n");
    else
        printf("FAILED: Invalid resource accepted\n");
}

void test_getrlimit()
{
    printf("\n========== TEST 5: GETRLIMIT ==========\n");

    struct rlimit lim;

    setrlimit(0, 7);

    getrlimit(0, &lim);

    if(lim.rlim_cur == 7)
        printf("SUCCESS: getrlimit returned correct value\n");
    else
        printf("FAILED: Expected 7 but got %ld\n", lim.rlim_cur);
}

int main()
{
    test_getrlimit();

    test_normal_limit();

    test_zero_limit();

    test_close_reuse();

    test_invalid_resource();

    exit(0);
}
