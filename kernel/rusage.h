#ifndef RUSAGE_H
#define RUSAGE_H

struct rusage {
  int pid;
  int state;
  uint64 runtime;
  uint64 waittime;
  uint64 sleeptime;
  int context_switches;
  char name[16];
};

#endif
