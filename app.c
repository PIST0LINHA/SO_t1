#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void app_loop(int id, int write_fd, int max_iter)
{
  int pc = 0;
  while(pc < max_iter)
  {
    printf("[A%d] PID = %d PC = %d\n", id, getpid(), pc);
    sleep(1);

    // simula I/O
    if(pc == 5)
    {
      printf("[A%d] Solicitando I/O\n", id);
      write(write_fd, "IO", 2);
      raise(SIGSTOP);
      printf("[A%d] Voltou do I/O\n", id);
    };

    if(pc == 12)
    {
      printf("[A%d] Solicitando I/O\n", id);
      write(write_fd, "IO", 2);
      raise(SIGSTOP);
      printf("[A%d] Voltou do I/O\n", id);
    };

    pc++;
  }
  printf("[A%d] finalizado\n", id);
}

