#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MB     (1024 * 1024)
#define PGSIZE 4096

int
main(int argc, char *argv[])
{
  struct meminfo before, after, final;

  meminfo(&before);
  printf("Before: free=%d KB, used=%d KB, total=%d KB\n",
         (int)(before.free_mem  / 1024),
         (int)(before.used_mem  / 1024),
         (int)(before.total_mem / 1024));

  // استخدمي sbrk مش malloc
  char *p = sbrk(10 * MB);
  if(p == (char*)-1){
    printf("stress: sbrk failed\n");
    exit(1);
  }
  // touch every page
  for(int i = 0; i < 10 * MB; i += PGSIZE)
    p[i] = 'x';

  meminfo(&after);
  printf("After alloc: free=%d KB, used=%d KB\n",
         (int)(after.free_mem / 1024),
         (int)(after.used_mem / 1024));

  if(after.free_mem < before.free_mem)
    printf("PASS: free decreased after allocation\n");
  else
    printf("FAIL: free did not decrease\n");

  // رجّعي الـ memory للـ kernel
  sbrk(-(10 * MB));

  meminfo(&final);
  printf("After free: free=%d KB, used=%d KB\n",
         (int)(final.free_mem / 1024),
         (int)(final.used_mem / 1024));

  if(final.free_mem > after.free_mem)
    printf("PASS: free increased after free()\n");
  else
    printf("FAIL: free did not increase\n");

  exit(0);
}
