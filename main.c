#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "app.h"
#include "kernel_sim.h"
#include "intercontroller_sim.h"

#define N_PROCESSOS 6
#define MAX_ITER 13

int main(void)
{
  PCB processos[N_PROCESSOS] = {0};
  pid_t kernel_pid;

  for(int i = 0; i < N_PROCESSOS; ++i)
  {
    pipe(processos[i].pipefd);
    pid_t pid = fork();
    if(pid == 0) 
    {
	  if(i == 2 || i == 5)
	  {
        close(processos[i].pipefd[0]);
        raise(SIGSTOP);
        app_loop(i + 1, processos[i].pipefd[1], MAX_ITER, 1);
        exit(0);
	  } else 
	  {
	    close(processos[i].pipefd[0]);
        raise(SIGSTOP);
	    app_loop(i + 1, processos[i].pipefd[1], MAX_ITER, 0);
	    exit(0);
	  }
	 }
    if(pid > 0) processos[i].pid = pid;
  };

  kernel_pid = fork();
  if(kernel_pid == 0)
  {
    kernel_init(processos);
    kernel_loop();
    exit(0);
  }

  pid_t interrupt_id = fork();
  if(interrupt_id == 0)
  {
    interrupt_controller(kernel_pid);
    exit(0);
  }

  for(int i = 0; i < N_PROCESSOS; ++i) wait(NULL);
  waitpid(kernel_pid, NULL, 0);
  waitpid(interrupt_id, NULL, 0);
  return 0;
}
