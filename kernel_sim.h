#ifndef KERNEL_SIM_H
#define KERNEL_SIM_H

#include <unistd.h>
#include <signal.h>

#define PROCESSOS 6

typedef enum {
  READY, 
  RUNNING, 
  BLOCKED } 
State;

typedef struct {
    pid_t pid;
    State state;
    int pipefd[2];  
}PCB;

void kernel_init(PCB *procs, int num_procs);
void kernel_loop();

#endif
