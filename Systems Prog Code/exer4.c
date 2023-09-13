/*
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Complier Used: Emacs
Program Description: This program creates a child process. The parent
                     and child perform different tasks.

 */

#include "headers.h"
#define BUF_SIZE 4096

int lower25, greater75;
pid_t process_pid;
pid_t child_pid;


void sigHandlerChild(int sig) // child signal handler
{
  if (sig == SIGTERM) // informs the child that its parent has died and needs to terminate itself
    {
      printf("\nParent process terminated... child terminating...\n");
      printf("Exiting...\n");
      exit(EXIT_SUCCESS);
    }
  if (sig == SIGALRM) // behavior function; creates random integer
    {
      int random; // generate random integer
      random = rand() % 100;
      printf("%d\n", random);
      if (random > 75) // if number is greater than 75
	{
	  kill(process_pid, SIGUSR2); // generate SIGUSR2 and send to parent
	}
      else if (random < 25) // if number is less than 25
	{
	  kill(process_pid, SIGUSR1); // generate SIGUSR1 and send to parent
	}
      else if (random > 47 && random < 52) // if number is between 48 and 51
	{
	  printf("Generated a number betweeen 48 and 51...Exiting...\n"); // child exists
	  exit(EXIT_SUCCESS); // Exit
	}
    }
}

void sigHandlerParent(int sig) // parent signal handler
{
  if (sig == SIGINT) // if sig == SIGINT
    {
      char input[BUF_SIZE];
      printf("Exit: Are you sure? (Y/n): "); // do you want to exit message
      scanf("%c\n", &input);
      if (input[0] == 'y' || input[0] == 'Y') // if user input is Y or y
	{
	  kill(child_pid, SIGTERM);
	  printf("\nGOOOOOD BYEEEE\n");
	  exit(EXIT_SUCCESS);
	}
      if (input[0] == 'n' || input[0] == 'N')
	{
	  printf("Okie dokie!\n");
	  return;
	}
    }
  if (sig == SIGCHLD) // if sig == SIGCHLD
    {
      int p = 0, status = 0;
      while ((p = waitpid(-1, &status,  WNOHANG)) > 0) // parent uses waitpid to clean up after children that have terminated
	{
	}
	  if (p == -1)
	    {
	      if (errno == ECHILD)
		{
		  printf("No child remains . . . Exiting . . . \n"); // no children remain message
		  exit(EXIT_SUCCESS); // Exit
		}
	      perror("Waitpid");
	      exit(EXIT_FAILURE);
	    }
    }
  
  if (sig == SIGUSR1) // if sig == SIGUSR1
    {
      lower25++;
      printf("The child generated %d values less than 25\n", lower25); // message X values are less than 25
      return;
    }
  if (sig == SIGUSR2) // if sig == SIGUSR2
    {
      greater75++;
      printf("The child generated %d values greater than 75\n", greater75); // message Y values are greater than 75
      return;
    }
}

int main(int argc, char *argv[])
{
  srand(time(NULL));
  struct sigaction saP; // parent signals
  struct sigaction saC; // child signals

  struct itimerval itmr; // timer stuff
  itmr.it_interval.tv_sec = 15; // 15 seconds
  itmr.it_interval.tv_usec = 0;
  itmr.it_value.tv_sec = 15;
  itmr.it_value.tv_usec = 0;

  // parent signal handlers initialize
  sigemptyset(&saP.sa_mask);
  saP.sa_handler = sigHandlerParent;
  saP.sa_flags = 0;

  // child signal handlers initialize
  sigemptyset(&saC.sa_mask);
  saC.sa_handler = sigHandlerChild;
  saC.sa_flags = 0;

  sigset_t block;
  sigemptyset(&block);
  sigaddset(&block, SIGINT);

  process_pid = getpid();
  printf("ProcessID is %d\n", process_pid);

  // parent signal error messages
  if (sigaction(SIGCHLD, &saP, NULL) == -1)
    {
      perror("SIGCHLD error");
      exit(EXIT_FAILURE);
    }
  if (sigaction(SIGUSR1, &saP, NULL) == -1)
    {
      perror("SIGUSR1 error");
      exit(EXIT_FAILURE);
    }
    if (sigaction(SIGUSR2, &saP, NULL) == -1)
    {
      perror("SIGUSR2 error");
      exit(EXIT_FAILURE);
    }
    if (sigaction(SIGINT, &saP, NULL) == -1)
      {
	perror("SIGINT error");
	exit(EXIT_FAILURE);
      }

    // child signal error messages
    switch(child_pid = fork())
      {
      case -1: //if child_pid == -1
	perror("Fork error");
      case 0: // if child_pid == 0
	if (sigaction(SIGALRM, &saC, NULL) == -1)
	  {
	    perror("SIGALRM error in child");
	    exit(EXIT_FAILURE);
	  }
	if (setitimer(ITIMER_REAL, &itmr, NULL) == -1)
	  {
	    perror("SETITIMER error in child");
	    exit(EXIT_FAILURE);
	  }
	if (sigaction(SIGTERM, &saC, NULL) == -1)
	  {
	    perror("SIGTERM error in child");
	    exit(EXIT_FAILURE);
	  }
	if (sigprocmask(SIG_BLOCK, &block, NULL) == -1)
	  {
	    perror("SIGPROCMASK error");
	  }
	while(1)
	  {
	    pause();
	  }
	break;
      default:
	while(1)
	  {
	    pause();
	  }
      }
} 
  
      
