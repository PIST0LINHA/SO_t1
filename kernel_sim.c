#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// para 6 processos
#define PROCESSOS 6

// estados do processo
typedef enum {
  READY,
  RUNNING,
  BLOCKED
} State;

// processo e meta dados
typedef struct {
  pid_t pid;
  State state;
  int pipefd[2];
} PCB;

PCB *processos;
int atual = 0;

void scheduler()
{
  int prox = (atual + 1) % PROCESSOS;
  while(processos[prox].state != READY) prox = (prox + 1) % PROCESSOS;
  
  if(processos[atual].state == RUNNING)
  {
    kill(processos[atual].pid, SIGSTOP);
    processos[atual].state = READY;
  }

  kill(processos[prox].pid, SIGCONT);
  processos[prox].state = RUNNING;
  atual = prox;
}

void irq0_handler(int sig)
{
  //printf("[KERNEL]IRQ0\n");
  scheduler();
}

void irq1_handler(int sig)
{
  //printf("[KERNEL]IRQ1\n");
  for(int i = 0; i < PROCESSOS; ++i)
  {
    if(processos[i].state == BLOCKED)
    {
      //printf("Desbloqueando A%d\n", i+1);
      processos[i].state = READY;
      break;
    }
  }
}

void kernel_init(PCB *processos1, int num) // processos1 para nao dar problema com processos
{
  processos = processos1;
}

void kernel_loop()
{
  signal(SIGUSR1, irq0_handler);
  signal(SIGUSR2, irq1_handler);

  processos[0].state = RUNNING;
  kill(processos[0].pid, SIGCONT);
  
  char buffer[10];

  while(1)
  {
    for(int i = 0; i < PROCESSOS; ++i)
    {
      int n = read(processos[i].pipefd[0], buffer, sizeof(buffer));
      if(n > 0)
      {
        buffer[n] = '\0';
        if(strcmp(buffer, "IO") == 0)
        {
          //printf("[KERNEL]A%d pediu I/O\n", n+1);
          processos[i].state = BLOCKED;
          scheduler();
        }
      }
    }
    usleep(100000); // antes usava sleep(1) e dava problema pra detectar pedido de I/O
  }
}
