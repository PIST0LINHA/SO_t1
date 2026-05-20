#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_PROCESSOS 6

void processos_aplicacoes(int id)
{
  int pc = 0;
  while(1)
  {
    printf("[A%d] PID = %d PC = %d\n", id, getpid(), pc++);
    sleep(1);
  }
}

