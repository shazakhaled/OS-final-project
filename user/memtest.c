#include "kernel/types.h"
#include "user/user.h"

void test_result(char *name, int condition) {
    printf("TEST: %s -> ", name);
    if(condition) {
        printf("PASSED [OK]\n");
    } else {
        printf("FAILED [X]\n");
    }
}

int main() {
    struct meminfo mi;
    
    printf("--- Running System Call Test Cases ---\n\n");

    // Case 1: Standard Call
    int res = meminfo(&mi);
    printf("Case 1: Valid Pointer\n");
    printf("  Expected: 0, Actual: %d\n", res);
    test_result("Return Code", res == 0);
    test_result("Memory Logic", (mi.free_mem + mi.used_mem == 134217728));

    printf("\n");

    // Case 2: Invalid Pointer (NULL)
    int res_null = meminfo((struct meminfo *)0);
    printf("Case 2: NULL Pointer\n");
    printf("  Expected: -1, Actual: %d\n", res_null);
    test_result("Security Check", res_null == -1);

    printf("\n------------------------------------\n");
    exit(0);
}
