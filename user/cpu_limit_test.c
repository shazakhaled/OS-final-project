#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  if(argc!=2)
  {
    printf("Please enter 1 number.");
    exit(1);
  }
  int n=atoi(argv[1]);
  if(n<=0) 
  {
    printf("Please enter a positive number.");
    exit(1);
  }
  int sum=0;
  struct rlimit limit;
  getrlimit(2,&limit);
  
  printf("CPU ticks remaining (before loop): %lu\n",limit.rlim_cur);
  for(int i = 1; i < n; i++){
    for(int j=1;j<n;j++)
    sum+=j*i;
  }
  getrlimit(2,&limit);
  
  printf("CPU ticks remaining (after loop): %lu\n",limit.rlim_cur);
  exit(0);
}
