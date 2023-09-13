/* Write a multiple concurrent process program that does the following
   1. Prompts the user for the number of integers to enter
   2. Prompts the user for each integer and writes them into 
      a file named data.dat
   3. Determines how many integers are > 100
   4. Determines how many integers are < 100
   5. Outputs the total count for each group.

   The program should perform this task in the following way:
   Create a producer child that:
   1. Prompts the user for the number of integers to enter
   2. Prompts the user for each integer and writes them into a file
      named data.dat
   Create a consumer child that:
1.	For each value in the file
a.	Determine if value > 100
b.	If the value > 100, signals the parent with SIGUSR1
   Create a consumer child that:
1.	For each value in the file
a.	Determines if value < 100
b.	If the value < 100, signals the parent with SIGUSR2
   Create a parent that: 
   1. Creates children 1 - 3
   2. Pauses. 

 */

/* Include files */
#include <stdio.h>
#include <stdlib.h>

/* Problem 1. Add the necessary include files for this program */
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

/* 
   Global variables:
   For this project I am storing the pid's of the three children
   and two integers that serve the role of flags and counters

 */

/* Problem 2. Declare the global variables for pid's of the three
   children and the two integers that serve the role of flags and
   counters. The first flag deals with the large count; the second
   flag deals with the small count.
 */
pid_t process1, process2, process3;
int flag1 = 0, flag2 = 0, flag3 = 0;
int lcounter = 0, scounter = 0;

#define BUF_SIZE 1024

union dtob
{
  int value;
  char bytes[4];
};

/* myPrint is a convenience function to use when we are in a signal 
   handler. This is because printf uses buffered I/O.
 */
void myPrint(const char *str)
{
  if (write(STDOUT_FILENO, str, strlen(str)) == -1)
    {
      perror("write");
      exit(EXIT_FAILURE);
    }
}

// imported function from Test 1 Program to print integers
int myPrintInt(const int val)
{
  char intStr[BUF_SIZE];
  sprintf(intStr, "%d", val);
  int fd = write(STDOUT_FILENO, intStr, strlen(intStr));
   if (fd == 0)
    {
      perror("Failed to print int.\n");
      return -1;
    }
}

/* Signal handlers
   We will need to implement at least two signal handlers. 
   One for the parent and a minimum of one for the children
 */

/* Problem 3. Define and write the signal handler for the parent */
void signalHandlerParent(int sig)
{
  int p = 0, status = 0;
  if (sig == SIGCHLD)
    {
      while((p = waitpid(-1, &status, WNOHANG)) > 0)
        {
	  if (p == process1)
            {
	      kill(process2, SIGUSR1);
	      kill(process3, SIGUSR2);
	      return;
            }
	}
      if (p == -1)
	{
	  if (errno == ECHILD)
	    {
	      myPrint("Smaller: ");
	      myPrintInt(scounter);
	      myPrint("\n");
	      myPrint("Larger: ");
	      myPrintInt(lcounter);
	      myPrint("\n");
	      exit(EXIT_SUCCESS);
	    }
	  else
	    {
	      perror("waitpid");
	      exit(EXIT_FAILURE);
	    }
	}
    }
  if (sig == SIGUSR1)
    {
      lcounter++;
      return;
    }
  if (sig == SIGUSR2)
    {
      scounter++;
      return;
    }
}


/* Problem 4. Define and write the signal handler for the children */
void signalHandlerChild(int sig)
{
  if (sig == SIGUSR1)
    {
      flag1 = 1;
      return;
    }
  if (sig == SIGUSR2)
    {
      flag2 = 1;
      return;
    }
}


/* Functions for each of the children. 
   We will be writing functions for each of the three children.
   This should make it easier to answer the questions on threads.
 */

/* Problem 5. Define and write the function for child 1. */
void producerChild1()
{
  int numEntries;
  myPrint("How many numbers would you like to enter? ");
  scanf("%d", &numEntries);

  int num;
  union dtob value;

  int f =  O_CREAT | O_TRUNC | O_RDWR;
  int fd = open("data.dat", f);
  if (fd == -1)
    {
      perror("open");
      exit(EXIT_FAILURE);
    }

  for (int i = 0; i < numEntries; i++)
    {
      myPrint("\nEnter value number ");
      myPrintInt(i + 1);
      myPrint(": ");
      scanf("%d", &num);
      value.value = num;
      if (write(fd, value.bytes, 4) != 4)
        {
	  perror("write");
	  exit(EXIT_FAILURE);
        }
    }
  close(fd);
  exit(EXIT_SUCCESS);
}


/* Problem 6. Define and write the function for child 2. */
void consumerChild2()
{
  struct sigaction saC2;
  saC2.sa_handler = signalHandlerChild;
  saC2.sa_flags = 0;
  if (sigaction(SIGUSR1, &saC2, NULL) == -1)
    {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }
  while (flag1 == 0)
    {
      pause();
    }

  union dtob *fileCont = malloc(4);
  int fd = open("data.dat", O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP |  S_IWGRP | S_IROTH | S_IWOTH);
  if (fd <= 0)
    {
      perror("open");
      exit(EXIT_FAILURE);
    }

  int readBytes = read(fd, fileCont, 4);
  pid_t parent = getppid();

  while (readBytes > 0)
    {
      if (fileCont->value > 100)
        {
          lcounter++;
	  kill(parent, SIGUSR1);
        }
      readBytes = read(fd, fileCont, 4);
    }
  myPrint("Total larger: ");
  myPrintInt(lcounter);
  myPrint("\n");
  close(fd);
  exit(EXIT_SUCCESS);
}


/* Problem 7. Define and write the function for child 3. */
void consumerChild3()
{
  struct sigaction saC3;
  saC3.sa_handler = signalHandlerChild;
  sigemptyset(&saC3.sa_mask);
  saC3.sa_flags = 0;
  if (sigaction(SIGUSR2, &saC3, NULL) == -1)
    {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }
  while (flag2 == 0)
    {
      pause();
    }

  union dtob *fileCont = malloc(4);
  int fd = open("data.dat", O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  if (fd <= 0)
    {
      perror("open");
      exit(EXIT_FAILURE);
    }

  int readBytes = read(fd, fileCont, 4);
  pid_t parent = getppid();

  while (readBytes > 0)
    {
      if (fileCont->value < 100)
	{
	  scounter++;
	  kill(getppid(), SIGUSR2);
        }
      readBytes = read(fd, fileCont, 4);
    }
  myPrint("Total smaller: ");
  myPrintInt(scounter);
  myPrint("\n");
  close(fd);
  exit(EXIT_SUCCESS);
}


/* This function forks a child and runs the function passed
   in after the child has successfully forked. I have provided
   it to make the code easier to read.
 */
pid_t hndlFork(void (*child)(void))
{
  pid_t p;
  p = fork();
  if (p == -1)
    {
      perror("fork");
      exit(EXIT_FAILURE);
    }
  if (p == 0)
    {
      child();
    }
  return p;
}

/* Problem 8: Define and write the function main */
int main(int argc, char *argv[])
{
  struct sigaction sa;
  sa.sa_handler = signalHandlerParent;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }

  process2 = hndlFork(consumerChild2);
  process3 = hndlFork(consumerChild3);

  if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }
  if (sigaction(SIGUSR2, &sa, NULL) == -1)
    {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }

  process1 = hndlFork(producerChild1);
  while(1)
    {
      pause();
    }

  exit(EXIT_SUCCESS);
}

