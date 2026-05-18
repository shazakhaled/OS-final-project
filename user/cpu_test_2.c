#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


int main(int argc, char *argv[]) {
  struct rlimit lim;
  int res = RLIMIT_CPU;

  // 1. Let the CPU work first to accumulate baseline ticks
  printf("Phase 1: Working to accumulate baseline CPU ticks...\n");
  
  volatile unsigned long long burn = 0;
  for(burn = 0; burn < 120000000ULL; burn++) {
    // Heavy loop to trigger clock interrupts and increase myproc()->cpu_ticks
  }

  // 2. Fetch the current tracking metrics via getrlimit
  getrlimit(res, &lim);
  
  // Based on your dynamic kernel math: lim.rlim_cur = max_limit - currently_used
  // Assuming a default max limit of 100 ticks:
  int default_max = 100; 
  int current_used = default_max - (int)lim.rlim_cur;

  printf("Baseline work completed.\n");
  printf("Current CPU ticks used: %d\n", current_used);

  // 3. Intentionally calculate a limit that is LESS than current_used
  // If we have used 15 ticks, we try to set a strict limit of 5 ticks.
  int illegal_limit = current_used - 5;
  if (illegal_limit <= 0) {
    illegal_limit = 1; // Safeguard to ensure it stays positive but still below usage
  }

  printf("Attempting illegal setrlimit with limit = %d...\n", illegal_limit);

  // 4. This call must fail inside your kernel switch block
  int success = setrlimit(res, illegal_limit);
  
  if(success < 0) {
    printf("\n[SUCCESS] setrlimit rejected the illegal limit!\n");
    printf("Kernel guard rail 'limit must be greater than CPU ticks used' worked perfectly.\n");
    exit(0);
  } else {
    printf("\n[ERROR] Kernel allowed setting a limit below current CPU usage!\n");
    printf("Check your guard rail comparison logic: if(rlim_max <= myproc()->cpu_ticks)\n");
    exit(1);
  }
}
