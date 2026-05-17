#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if(argc != 2){
    printf("Usage: setrlimit <ticks_limit>\n");
    exit(1);
  }

  int n = atoi(argv[1]);
  struct rlimit limit;
  int new_limit;

  new_limit = n;
  

  // Hena bn-nada7 el-syscall elly enti 3amaltiha
  if(setrlimit(1, new_limit) < 0){
    printf("setrlimit failed\n");
    exit(1);
  }

  getrlimit(1, &limit);
  printf("New limit set to: %lu ticks\n", limit.rlim_max);

  printf("Running heavy loop to test limit...\n");
  int sum = 0;
  for(int i = 0; i < 100; i++){
    for(int j = 0; j < 1000000; j++){
      sum += i * j;
    }
  }

  getrlimit(1, &limit);
  printf("Loop finished. Remaining ticks: %lu\n", limit.rlim_cur);
  printf("Sum: %d\n", sum);

  exit(0);
}
