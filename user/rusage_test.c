#include "kernel/types.h"
#include "kernel/rusage.h"
#include "user/user.h"

int main(){
 
 struct rusage info;
 getrusage(&info);

 printf("PID");




}
