#include "kernel/types.h"
#include "user/user.h"


#define MAX_INODE_PAIRS 64

struct inode_pair {
  int inum;
  int dev;

  int offset_a;
  int offset_b;

  int pid_a;
  int fd_a;

  int pid_b;
  int fd_b;
};

struct inode_pairs {
  int npairs;
  struct inode_pair pairs[MAX_INODE_PAIRS];
};

int
main(void)
{
  struct inode_pairs res;

  if(inodepairs(&res) < 0){
    printf("syscall failed\n");
    exit(1);
  }

  for(int i = 0; i < res.npairs; i++){
    struct inode_pair *p = &res.pairs[i];

    printf("inode=%d dev=%d\n", p->inum, p->dev);

    printf("  A: pid=%d fd=%d offset=%d\n",
      p->pid_a, p->fd_a, p->offset_a);

    printf("  B: pid=%d fd=%d offset=%d\n",
      p->pid_b, p->fd_b, p->offset_b);
  }

  exit(0);
}
