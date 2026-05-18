#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void cleanup(int fds[], int n)
{
    for(int i = 0; i < n; i++) {
        int result;
        if(fds[i] >= 0) //it has a valid file open
        {
          printf("Closing fds %d.\n", fds[i]);
          result=close(fds[i]);
          if(result<0)
          {
          printf("error closing file %d", fds[i]);
          exit(1);
          }
          else fds[i]=-1;
        }

    }
}
int main(int argc, char *argv[]) {

 if(argc!=2)
  {
    printf("Please enter 1 number.");
    exit(1);
  }
  int n=atoi(argv[1]);
//  if(n<=0) 
  //{
  //  printf("Please enter a positive number.");
  //  exit(1);
//  }
  struct rlimit lim;
  int res = 0; 

  getrlimit(RLIMIT_NOFILE, &lim);
  printf("Default - max: %d\n", (int)lim.rlim_max);

  int new_limit =n;
  int success=setrlimit(res, new_limit);
  if(success<0)
    exit(1);
  getrlimit(res, &lim);
  printf("New Limit - max: %d\n", (int)lim.rlim_max);
  
  int fds[20];
  for(int i = 0; i < 20; i++) {
      fds[i] = -1;
  } //initialize all to -1 to ensure no garbage values or values >=0
  
  for(int i = 0; i < n+4; i++)
  {
    fds[i]=open("README", 0);
    if(fds[i] < 0)
    {
      printf("LIMIT EXCEEDED at iteration: %d\n", i+1);
      printf("Will close all files previously opened.\n");
      cleanup(fds,i);
//     exit(0);
    }
    else
    printf("Opened file %d successfully.\n", fds[i]);
  }
exit(0);
}
