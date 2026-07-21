typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long uint64;

typedef uint64 pde_t;

struct meminfo {
  uint64 total_mem;
  uint64 free_mem;
  uint64 used_mem;
};

struct rlimit{
   uint64 rlim_cur;
   uint64  rlim_max;
};
