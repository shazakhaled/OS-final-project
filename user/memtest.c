#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PGSIZE 4096
#define MB     (1024 * 1024)

static int passed = 0;
static int failed = 0;

void
test_result(char *name, int condition)
{
  if(condition){
    printf("  PASS: %s\n", name);
    passed++;
  } else {
    printf("  FAIL: %s\n", name);
    failed++;
  }
}

// Case 1: الـ syscall شغالة وبترجع قيم منطقية
void
test_valid_pointer(void)
{
  printf("Case 1: Valid pointer\n");
  struct meminfo mi;
  int res = meminfo(&mi);
  test_result("syscall returns 0",     res == 0);
  test_result("free_mem > 0",          mi.free_mem > 0);
  test_result("used_mem > 0",          mi.used_mem > 0);
  test_result("total_mem > 0",         mi.total_mem > 0);
  test_result("free + used == total",  mi.free_mem + mi.used_mem == mi.total_mem);
  test_result("free_mem page-aligned", mi.free_mem % PGSIZE == 0);
  test_result("total <= 128MB",        mi.total_mem <= 128 * MB);
  printf("\n");
}

// Case 2: NULL pointer
void
test_null_pointer(void)
{
  printf("Case 2: NULL pointer\n");
  int res = meminfo((struct meminfo *)0);
  test_result("returns -1 on NULL", res == -1);
  printf("\n");
}

// Case 3: bad pointer
void
test_bad_pointer(void)
{
  printf("Case 3: Bad pointer\n");
  int res = meminfo((struct meminfo *)0xFFFFFFFF);
  test_result("returns -1 on bad pointer", res == -1);
  printf("\n");
}

// Case 4: الـ malloc بيقلل الـ free memory
void
test_alloc_decreases(void)
{
  printf("Case 4: Allocation decreases free memory\n");
  struct meminfo before, after;
  meminfo(&before);

  char *p = malloc(4 * MB);
  if(p == 0){ printf("  SKIP: malloc failed\n\n"); return; }
  for(int i = 0; i < 4 * MB; i += PGSIZE)
    p[i] = 1;

  meminfo(&after);
  test_result("free decreased",      after.free_mem < before.free_mem);
  test_result("used increased",      after.used_mem > before.used_mem);
  test_result("total unchanged",     after.total_mem == before.total_mem);
  test_result("decrease >= 4MB",     before.free_mem - after.free_mem >= 4 * MB);

  free(p);
  printf("\n");
}

// Case 5: الـ free() بيزود الـ free memory
void
test_free_increases(void)
{
  printf("Case 5: Free increases free memory\n");
  struct meminfo before, after;

  char *p = malloc(4 * MB);
  if(p == 0){ printf("  SKIP: malloc failed\n\n"); return; }
  for(int i = 0; i < 4 * MB; i += PGSIZE)
    p[i] = 1;

  meminfo(&before);
  free(p);
  meminfo(&after);

  test_result("free increased",  after.free_mem > before.free_mem);
  test_result("used decreased",  after.used_mem < before.used_mem);
  test_result("total unchanged", after.total_mem == before.total_mem);
  printf("\n");
}

// Case 6: استدعائين متتاليين
void
test_consistency(void)
{
  printf("Case 6: Consistency between two calls\n");
  struct meminfo a, b;
  meminfo(&a);
  meminfo(&b);
  uint64 diff = a.free_mem > b.free_mem
                ? a.free_mem - b.free_mem
                : b.free_mem - a.free_mem;
  test_result("differ by at most 1 page", diff <= PGSIZE);
  printf("\n");
}

// Case 7: allocations متعددة
void
test_multiple_allocs(void)
{
  printf("Case 7: Multiple allocations\n");
  struct meminfo before, after;
  char *ptrs[4];
  int n = 4;

  meminfo(&before);
  for(int i = 0; i < n; i++){
    ptrs[i] = malloc(MB);
    if(ptrs[i] == 0){ printf("  SKIP: malloc failed\n\n"); return; }
    for(int j = 0; j < MB; j += PGSIZE)
      ptrs[i][j] = 1;
  }
  meminfo(&after);

  test_result("free decreased by >= 4MB",
              before.free_mem - after.free_mem >= (uint64)(n * MB));
  test_result("total unchanged",
              after.total_mem == before.total_mem);

  for(int i = 0; i < n; i++) free(ptrs[i]);
  printf("\n");
}

int
main(void)
{
  printf("=== meminfo syscall test suite ===\n\n");

  test_valid_pointer();
  test_null_pointer();
  test_bad_pointer();
  test_alloc_decreases();
  test_free_increases();
  test_consistency();
  test_multiple_allocs();

  printf("==================================\n");
  printf("Results: %d passed, %d failed\n", passed, failed);
  if(failed == 0)
    printf("ALL TESTS PASSED\n");
  else
    printf("SOME TESTS FAILED\n");

  exit(failed == 0 ? 0 : 1);
}
