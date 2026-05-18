#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  struct rlimit lim;
  int res = RLIMIT_CPU;

  if(argc != 2) {
    printf("Usage: cpu_test <tick_limit>\n");
    exit(1);
  }
  int n = atoi(argv[1]);

  // 1. Check Default Limits
  getrlimit(res, &lim);
  printf("Default CPU Limit - max: %d, cur: %d\n", (int)lim.rlim_max, (int)lim.rlim_cur);

  // 2. Enforce the new limit via setrlimit
  int success = setrlimit(res, n);
  if(success < 0) {
    printf("setrlimit failed! Guard rails working as expected.\n");
    exit(1);
  }

  getrlimit(res, &lim);
  printf("New CPU Limit Enforced - max: %d\n", (int)lim.rlim_max);

  // 3. Start a heavy workload to actively burn CPU ticks
  printf("\nStarting heavy CPU workload loop to consume ticks...\n");
  
  volatile unsigned long long counter = 0;
  int last_known_remaining = -1;

  while(1) {
    // Perform some dummy mathematical operations to waste CPU cycles
    counter += 1;
    if (counter % 5000000ULL == 0) {
      
      // 4. Track the dynamic countdown using your custom getrlimit
      getrlimit(res, &lim);
      
      // Only print when the tick count actually changes to avoid flooding the console
      if ((int)lim.rlim_cur != last_known_remaining) {
        last_known_remaining = (int)lim.rlim_cur;
        printf("CPU ticks remaining before process termination: %d\n", last_known_remaining);
      }
      
      // If your kernel dynamically sets rlim_cur to 0 when exceeded, 
      // or if you want an explicit user-side warning right before the crash:
      if (last_known_remaining <= 0) {
         printf("CRITICAL: Process is at or past its allowed CPU limit!\n");
      }
    }
  }

  // Under proper kernel enforcement, the process will be killed by the kernel 
  // long before it can ever reach this line!
  exit(0);
}
