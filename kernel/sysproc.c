#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    if(addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


uint64
sys_meminfo(void)
{
  uint64 addr;
  struct meminfo mi;
  
  argaddr(0, &addr); 

  mi.free_mem = count_free_mem();
  mi.used_mem = (128 * 1024 * 1024) - mi.free_mem;

  if(copyout(myproc()->pagetable, addr, (char *)&mi, sizeof(mi)) < 0)
    return -1;

  return 0;
}

uint64
sys_getrlimit(void)
{
  int resource;
  uint64 user_rlim_addr;
  struct rlimit lim;

  // في XV6 الـ functions دي void، بنناديها والـ Kernel بيملى المتغيرات
  argint(0, &resource);
  argaddr(1, &user_rlim_addr);

  // default init
  lim.rlim_cur = 0;
  lim.rlim_max = 0;

  switch(resource) {
    case RLIMIT_NOFILE:{
// 1. بنعد الملفات اللي العملية فاتحاها فعلياً دلوقتي
int currently_open = get_proc_open_files(myproc());

// 2. بنجيب الليميت الخاص بالعملية دي (مش الـ 16 الثابتة)
int max = myproc()->nofile_max; 

lim.rlim_max = max;;

// 3. بنحسب الـ remaining (الفاضل كام ملف مسموح بفتحه)
if(currently_open >= max)
lim.rlim_cur = 0;
else
lim.rlim_cur = max - currently_open;
break;
}
    case RLIMIT_MEMORY:
      break;

    case RLIMIT_CPU:
{ uint64 max = myproc()->cpu_ticks_max; 
uint64 used = myproc()->cpu_ticks;

lim.rlim_max = max;

if(used >= max)
{
lim.rlim_cur = 0;
}
else
{
lim.rlim_cur = max - used;
}
break;
}
default:
return -1;
}

  struct proc *p = myproc();

  if(copyout(p->pagetable, user_rlim_addr, (char *)&lim, sizeof(lim)) < 0)
    return -1;

  return 0;
}



uint64
sys_setrlimit(void)
{
int resource;
uint64 user_rlim_addr;
struct rlimit lim;

argint(0, &resource);
argaddr(1, &user_rlim_addr);

if(copyin(myproc()->pagetable, (char*)&lim,
user_rlim_addr, sizeof(lim)) < 0)
return -1;

switch(resource) {

case RLIMIT_CPU:{
if(lim.rlim_max > MAX_CPU_TICKS){
printf("setrlimit: exceeds system max %d\n", MAX_CPU_TICKS);
return -1;
}
if(lim.rlim_max == 0){
printf("setrlimit: limit must be > 0\n");
return -1;
}
myproc()->cpu_ticks_max = lim.rlim_max;
return 0;
}
case RLIMIT_NOFILE:{
if(lim.rlim_max > NOFILE){
printf("setrlimit: exceeds system NOFILE limit %d\n", NOFILE);
return -1;
}
// قاعدة 2: ما ينفعش يكون الليميت 0 أو سالب
if(lim.rlim_max <= 0){
printf("setrlimit: limit must be > 0\n");
return -1;
}
// قاعدة 3: ما ينفعش نقلل الليميت عن عدد الملفات اللي العملية فاتحاها فعلياً دلوقتي
int currently_open = get_proc_open_files(myproc());
if(lim.rlim_max < currently_open){
printf("setrlimit: can't set below currently open files (%d)\n", currently_open);
return -1;
}
myproc()->nofile_max = lim.rlim_max;
return 0;
}

default:
return -1;
}
}

