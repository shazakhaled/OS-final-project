#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



int main(int argc, char *argv[]) {
struct rlimit lim;
// جربي رقم 1 لو 0 منفعش، حسب تعريفك في الـ Kernel
int res = 0; 

getrlimit(res, &lim);
printf("Default - max: %d\n", (int)lim.rlim_max);

int new_limit =5;
setrlimit(res, new_limit);

getrlimit(res, &lim);
printf("New Limit - max: %d\n", (int)lim.rlim_max);

for(int i = 0; i < 10; i++){
if(open("README", 0) < 0){
printf("PASSED: Limit hit at i=%d\n", i);
exit(0);
}
printf("Opened file %d\n", i);
}
exit(0);
}
