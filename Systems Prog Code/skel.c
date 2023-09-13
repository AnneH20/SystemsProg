/* 
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Emacs
Program Description: This is the program portion of Test 1
*/



/* All of the includes for the project go here … 
 * those includes must support signals, interval timers,
 * read, open, and close. You will also need support for 
 * error numbers
 */

#include <stdio.h>
#include <stdlib.h>

/* Problem 1: Complete the lsit of include files */
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define BUF_SIZE 1024

static int timed;

/* the myPrint function takes a constant c-string
 * and outputs it to STDOUT using write
 * myPrint returns 0 on failure and 1 on success
 */
int myPrint(const char *str)
{
/* Problem 2: Implement myPrint using write */
  int fd = write(STDOUT_FILENO, str, strlen(str));
  if (fd == 0)
    {
      perror("Failure to print string.\n");
      return -1;
    }
}

/* the myPrintInt function takes a constant int
 * and outputs it to STDOUT using write
 * myPrintInt returns 0 on failure and 1 on success
 */
int myPrintInt(const int val)
{
/* Problem 3: Implement myPrintInt, you must use write or another function implemented in this program */
  char intStr[BUF_SIZE];
  sprintf(intStr, "%d", val);
  int fd = write(STDOUT_FILENO, intStr, strlen(intStr));
   if (fd == 0)
    {
      perror("Failed to print int.\n");
      return -1;
    }
}

/* implement the signal handler below ...
 * use the provided prototype
 * It must handle two signals, SIGINT and SIGALRM
 */
void signalHandler(int sig)
{
/* Problem 4: Implement the signalHandler */
  char input[BUF_SIZE];
  if (sig == SIGINT)
    {
      myPrint("Do you want to exit? ");
      read(STDIN_FILENO, input, BUF_SIZE);
      if (input[0] == 'Y')
	{
	  myPrint("Dang this thing works.\n"); /* Message confirming that we are exiting */
	  exit(EXIT_SUCCESS);
	}
      if (input[0] == 'n')
	{
	  myPrint("You must really love tests.\n"); /* Message acknowledging that we don't want to exit*/
	  return;
	}
    }
  if (sig == SIGALRM)
    {
      timed = 1;
    }
}

/* This function reads a line of text from a file
 * we are simulating another functions behavior here ..
 * Return a 0 if no characters were read, return a 1 otherwise
 */
int readLine(int fd, char *line)
{
  /* Problem 5: Implement readLine as described above */
  int numRead = 1;
  int i = 0;
  char usrInput[BUF_SIZE];
  while (numRead > -1)
    {
      if (numRead = read(fd, usrInput, 1) == 0)
	{
	  return 0;
	}
      if (usrInput[0] == '\n')
	{
	  line[i] = '\0';
	  break;
	}
      else
	{
	  line[i] = usrInput[0];
	  i++;
	}
    }
  return 1;
}

/* This function reads a question answer pairing
 * from the provided pair of file descriptors
 * It returns 0 if the files are empty
 * and 1 if if successfully reads the pairing
 */

int readQA(int questFd, int ansFd, char *quest, char *ans)
{
  if (readLine(questFd,quest) == 0) return 0; 
  if (readLine(ansFd, ans) == 0) return 0;
  return 1;
}

int main(int argc, char *argv[])
{
  int numRead = 0;
  int numWrite = 0;
  int question = 1;
  int correct = 0;
  char buf[BUF_SIZE];
  char quest[BUF_SIZE];
  char ans[BUF_SIZE];
  int questFd, ansFd;

 /* Problem 6a: Declare the structures for the signals and timers */
  struct sigaction sa;
  struct itimerval tvOn; /* active delay */
  struct itimerval tvOff; /* disabled delay */

 /* Problem 6b: Initialize the structure for the signal handlers */
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = signalHandler;
  sa.sa_flags = 0;
  
/* Problem 6c: Initialize the structure for the 30 sec timer */
  tvOn.it_value.tv_sec = 30;
  tvOn.it_value.tv_usec = 0;
  tvOn.it_interval.tv_sec = 0;
  tvOn.it_interval.tv_usec = 0;
  
/* Problem 6d: Initialize the structure for the 0 sec timer */
  tvOff.it_interval.tv_sec = 0;
  tvOff.it_interval.tv_usec = 0;
  tvOff.it_value.tv_sec = 0;
  tvOff.it_value.tv_usec = 0;
  
/* Problem 7: Associate the SIGINT and SIGALRM with their signal handler */
   if(sigaction(SIGINT, &sa, NULL) == -1)
    {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }
  if (sigaction(SIGALRM, &sa, NULL) == -1)
    {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }
  
/* Problem 8: Open the files quest.txt and ans.txt */
  questFd = open("quest.txt", O_RDWR);
  ansFd = open("ans.txt", O_RDWR);
  
   /* this loop handles the Q/A stuff 
    * I have included some of it to make you life simpler 
    * I have also left some commnents to assist you as well 
    *
    * read the first question, answer pairing prior to entering the loop
    */
    readQA(questFd, ansFd, quest, ans);
  while (1)
    {
      /* output the current question */
      myPrint("#");
      myPrintInt(question);
      myPrint(" ");
      myPrint(quest);
      myPrint("? ");
      /* we will set the timedout flag to 0, since it hasn't yet */
      timed = 0;

      /* now set the interval timer prior to reading in the user's response */

 /*  Problem 9: Set the interval timer to it's active delay structure */
      if (setitimer(ITIMER_REAL, &tvOn, NULL) == -1)
	{
	  perror("setitimer");
	  exit(EXIT_FAILURE);
	}

      /* read in the user's response, this is systems programming so don't use 
       * scanf */

 /* Problem 10: Read a buffer from STDIN */
     numRead =  read(STDIN_FILENO, buf, BUF_SIZE);

      /* test to see if the user responded and if an error has occurred 
       * an error can actually occur as part of the functionality of this program 
       * both SIGINT and SIGALRM will cause the read call to return a -1 
       * this can be recognized by testing for a specific error number ...  
       * The error is known as interrupted systems call 
       * Should this be the case, do the following .... 
       * If the read call was interrupted and the timedout flag isn't set 
       * just continue 
       * if the timedout flag was set, inform the user that time is up 
       * then go to the next question
       */
      if (numRead == 0) break;
      if (numRead == -1)
	{
	  if (errno == EINTR)
	    {
	      if (timed)
		{
		  myPrint("\nTime's up, next question\n");
		  if (readQA(questFd, ansFd, quest, ans) == 0) break;
		  question++;
		}
	      continue;
	    }
	  perror("read");
	  exit(EXIT_FAILURE);
	}
      /* disable the timer here */
/* Problem 11: Disable the time by setting it to tvOff */
      if (setitimer(ITIMER_REAL, &tvOff, NULL) == -1)
	{
	  perror("setitimer");
	  exit(EXIT_FAILURE);
	}
      
      /* we will convert the buf being read in to a c-string by tacking on a 0 */
	buf[numRead-1] = 0;
	/* check the answer */
	if (strcmp(buf,ans) == 0)
	  {
	    correct++;
	    myPrint("\ncorrect\n");
	  } else
	  {
	    myPrint(ans);
	    myPrint("\nwrong\n");
	  }
	/* read the next question .. break if there are no more questions */
	if (readQA(questFd, ansFd, quest, ans) == 0) break;

	question++;
    }

  myPrint("final score is ");
  myPrintInt(correct);
  myPrint(" out of ");
  myPrintInt(question);
  myPrint("\n");

/* Problem 12: Close both files */
  close(questFd);
  close(ansFd);
}
