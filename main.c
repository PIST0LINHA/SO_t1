#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "app.h"

#define N_PROCESSOS 6

int main(void)
{
  pid_t processos[N_PROCESSOS] = {0}; 

  for(int i = 0; i < N_PROCESSOS; ++i)
  {
    processos[i] = fork();
    if(processos[i] == 0)
    {
      processos_aplicacoes(i + 1);
      exit(0);
    }
  }

  while(1) sleep(10);

  return 0;
}
