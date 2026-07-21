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
          //randomly decide whether to close or not  
          //does NOT close odd i
          //to ensure fd[0] is always closed in case limit is 4
          if((i%2)==1)
          {
            printf("Randomly will not close file %d\n",fds[i]);
            continue;
          }
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
  struct rlimit lim;
  getrlimit(RLIMIT_NOFILE, &lim);
  printf("Number of files we can open: %d\n", (int)lim.rlim_cur);
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
  
  int pid=fork();
  if(pid < 0)
  {
    printf("Fork failed\n");
    exit(1);
  }
  if(pid == 0) {
    // child
    printf("Inside child\n");
    struct rlimit child_lim;
    getrlimit(RLIMIT_NOFILE, &child_lim);
    printf("Inherited (rlim_cur): %d\n", (int)child_lim.rlim_cur);
    printf("Inherited hard limit (rlim_max): %d\n", (int)child_lim.rlim_max);
    exit(0);
  }
  else
  {
  wait(0);
  printf("INSIDE PARENT\n");
  int fds[20];
  for(int i = 0; i < 20; i++) {
      fds[i] = -1;
  } //initialize all to -1 to ensure no garbage values or values >=0
  
  for(int i = 0; i < n+4; i++)
  {
    printf("Iteration: %d\n", i+1);
    fds[i]=open("README", 0);
    if(fds[i] < 0)
    {
      printf("LIMIT EXCEEDED at iteration: %d\n", i+1);
      printf("Will close some previously opened files.\n");
      cleanup(fds,i);
    }
    else
    printf("Opened file %d successfully.\n", fds[i]);
  }
exit(0);

  }
  }
