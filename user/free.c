#include "kernel/types.h"
#include "user/user.h"

void
print_memory_bar(int percent)
{
  printf("Usage: [");
  for(int i = 0; i < 20; i++){
    if(i < (percent / 5)) printf("#");
    else                   printf(".");
  }
  printf("] %d%%\n", percent);
}

int
main(int argc, char *argv[])
{
  struct meminfo mi;

  if(meminfo(&mi) < 0){
    printf("free: error\n");
    exit(1);
  }

  int usage_percent = (int)((mi.used_mem * 100) / mi.total_mem);

  printf("\n--- XV6 System Memory Statistics ---\n\n");
  printf("Total RAM\tUsed Memory\tFree Memory\n");
  printf("%d MB\t\t%d KB\t\t%d KB\n",
         (int)(mi.total_mem / (1024 * 1024)),
         (int)(mi.used_mem  / 1024),
         (int)(mi.free_mem  / 1024));
  printf("\n");
  print_memory_bar(usage_percent);
  printf("------------------------------------\n\n");
  exit(0);
}
