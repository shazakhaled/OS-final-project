#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc)
{
  int sum=0;
  struct rlimit limit;
  getrlimit(2,&limit);
  
  printf("CPU ticks remaining (before loop): %lu\n",limit.rlim_cur);
  for(int i = 1; i < argc; i++){
    sum+=i;
  }
  getrlimit(2,&limit);
  
  printf("CPU ticks remaining (after loop): %lu\n",limit.rlim_cur);
  exit(0);
}
