#include <unistd.h>
#include <signal.h>

void interrupt_controller(pid_t kernel_pid)
{
  int counter = 0;

  while(1)
  {
    //timeslice
    sleep(1);

    //IRQ0
    kill(kernel_pid, SIGUSR1);

    counter++;

    //IRQ1 
    if(counter % 3 == 0)
    {
      kill(kernel_pid, SIGUSR2);
    }
  }
}
