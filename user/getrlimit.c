#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){

struct rlimit lim;

if(getrlimit(0, &lim) < 0){
printf("Error calling getrlimit\n");
exit(1);
}

printf("Soft-File limit for this process is: %d\n" ,(int) lim.rlim_cur);
printf("Hard-File limit for this process is: %d\n" , (int) lim.rlim_max);

exit(0);
}

