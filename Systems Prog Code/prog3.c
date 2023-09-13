/*
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Complier Used: Emacs
Program Description: This program combines what we did in
Program 2 with what we did in Exercise 4
 */

#include "headers.h"

#define BUF_SIZE 4096

int fd, lastAlert, n;
pid_t process_pid;
pid_t child_pid;
char* filename = "flowData.dat";
time_t start;
time_t end;
int flag = 0;

union dtob
{
  int value;
  char bytes[4];
};



void sigHandlerChild(int sig) // child signal handler
{
  if (sig == SIGTERM) // informs the child that its parent has died and needs to terminate itself
    {
      printf("\nParent process terminated...child terminating...\n");
      printf("Exiting...\n");
      exit(EXIT_SUCCESS);
    }
  if (sig == SIGALRM) // behavior function
    {
      union dtob *fileCont = malloc(2);
      if (read(fd, fileCont, 2) != 0)
	{
	  printf("Current value: %d\n", fileCont->value); // testing current value
	  if (551 > fileCont->value)
	    {
	      kill(process_pid, SIGUSR1); // generate SIGUSR1 and send to parent
	      return;
	    }
	  if (1449 < fileCont->value)
	    {
	      kill(process_pid, SIGUSR2); // generate SIGUSR2 and send to parent
	      return;
	    }
	}
      else
	{
	  printf("End of file. Terminating...\n"); 
	  exit(EXIT_SUCCESS);
	}
    }
}

void sigHandlerParent(int sig) // parent signal handler
{
  double elapsed;
  
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
	      printf("Warning! Pressure control monitoring has been terminated, exiting the system.\n"); // no children remain message
	      exit(EXIT_SUCCESS); // Exit
	    }
	  perror("Waitpid");
	  exit(EXIT_FAILURE);
	}
    }
  
  if (sig == SIGUSR1) // if sig == SIGUSR1
    {
      time(&end);
      elapsed = difftime(end, start);
      if (flag < 1)
	{
	  elapsed = 0.000000;
	}
      printf("Warning! Line pressure is dangerously low. It has been %f seconds since the previous warning.\n", elapsed); // message x seconds since last alert
      time(&start);
      flag++;
      return;
    }
  if (sig == SIGUSR2) // if sig == SIGUSR2
    {
      time(&end);
      elapsed = difftime(end, start);
      if (flag < 1)
	{
	  elapsed = 0.000000;
	}
      printf("Warning! Line pressure is dangerously high. It has been %f seconds since the previous warning.\n", elapsed); // message x seconds since last alert
      time(&start);
      flag++;
      return;
    }
}

int main(int argc, char *argv[])
{
  int num;
  char buf[BUF_SIZE];
  
  time(&start);
  struct sigaction saP; // parent signals
  struct sigaction saC; // child signals

  struct itimerval itmr; // timer stuff
  itmr.it_interval.tv_sec = 5; // 5 seconds
  itmr.it_interval.tv_usec = 0;
  itmr.it_value.tv_sec = 5;
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
  // printf("ProcessID is %d\n", process_pid);

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
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	  {
	    perror("Open error");
	    exit(EXIT_FAILURE);
	  }
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
	printf("Enter an input.\n");
	char *input = ">>";
	while(1)
	  {
	    write(STDOUT_FILENO, input, 2);
	    num = read(STDIN_FILENO, buf, BUF_SIZE);
	    write(STDOUT_FILENO, buf, num);
	  }
      }
} 
