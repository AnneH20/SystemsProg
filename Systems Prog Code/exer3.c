/* 
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Emacs
Program Description: This program implements a modified version of the 
wc command using I/O system calls.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#define BUF_SIZE 1024


int main(int argc, char *argv[])
{
  int fd;
  
  char filename[BUF_SIZE];
  char buf[BUF_SIZE];
  char buffer2[BUF_SIZE];
  
  int cflag1 = 0;
  int wflag1 = 0;
  
  int fflag = 0;
  
  int cflag2 = 0;
  int wflag2 = 0;

  int wcflag = 0;
  char wctest[BUF_SIZE] = "wc";

  if (argc == 2)
    {
      if (strncmp(wctest, argv[1], 2) == 0)
	{
	  wcflag = 1;
	}
    }
  if (argc == 3)
    {
      if (strncmp(wctest, argv[1], 2) == 0)
	{
	  wcflag = 1;
	}
      
      if (strncmp("-", argv[2], 1) == 0)// check 2nd arguement for "-"
	{
	  if (strncmp("-w", argv[2], 2) == 0) // check for "-w"
	    {
	      wflag1 = 1;
	    }
	  if (strncmp("-c", argv[2], 2) == 0) // check for "-c"
	    {
	      cflag1 = 1;
	    }
	}
      else
	{
	  sscanf(argv[2], "%s", filename); // check for filename
	  fflag = 1;
	}  
    }
	      
  if (argc == 4)
    {
      if (strncmp(wctest, argv[1], 2) == 0)
	{
	  wcflag = 1;
	}
      
      if (strncmp("-", argv[2], 1) == 0) // check 2nd argument for "-"
	{
	  if (strncmp("-w", argv[2], 2) == 0) // check for "-w"
	    {
	      wflag1 = 1;
	    }
	  if (strncmp("-c", argv[2], 2) == 0) // check for "-c"
	    {
	      cflag1 = 1;
	    }
	  if (strncmp("-", argv[3], 1) == 0) // "check 3rd arguement for "-"
	    {
	      printf("dsgk\n");
	      if (cflag1 != 1)
		{
		  if (strncmp("-c", argv[3], 2) == 0) // check for "-c"
		    {
		      cflag2 = 1;
		      printf("cflag2\n");
		    }
		}
	      if (wflag1 != 1)
		{
		  if (strncmp("-w", argv[3], 2) == 0) // check for "-w"
		    {
		      wflag2 = 1;
		    }
		}
	    }
	  else
	    {
	      sscanf(argv[3], "%s", filename); // check for filename
	      fflag = 1;
	    }
	}
    }

      fd = open(filename, O_RDONLY); // open file
      if (fd != -1)
	{
	  int i = 0;
	  int c;
	  int SPACE;
	  int test = 1;

	   while (test > -1)
	    {
	      if (test = read(fd, buf, 1) == 0) // read
		{
		  buffer2[i] = '\0';
		  break;
		}
	      buffer2[i] = buf[0];
	      i++;
	    }

	  if (cflag1 == 1 || cflag2 == 1)
	    {
	      c = strlen(buffer2);
	    }

	  if (wflag1 == 1 || wflag2 == 1) // check for wflag1
	    {
	      for (int i = 0; i < strlen(buffer2); i++)
		{
		  if (buffer2[i] == ' ' || buffer2[i] == '\n')
		    {
		      SPACE++;
		    }
		}
	    }

	  if ( wcflag == 1 && wflag1 == 0 && cflag1 == 0)
	    {
	      c = strlen(buffer2);
	      for (int i = 0; i < strlen(buffer2); i++)
		{
		  if (buffer2[i] == ' ' || buffer2[i] == '\n')
		    {
		      SPACE++;
		    }
		}
	      
	      printf("Number of characters: ");
	      printf("%d\n", c); // print number of characters
	      printf("Number of words: ");
	      printf("%d\n", SPACE); // print number of words
	    }


	  
	  if (cflag1 == 1 || cflag2 == 1) // check for cflags
	    {
	      printf("Number of characters: ");
	      printf("%d\n", c);
	    }
      
	  if (wflag1 == 1 || wflag2 == 1) // check for wflags
	    {
	      printf("Number of words: ");
	      printf("%d\n", SPACE);
	    }
	}

  if (fd == -1)
    {
      int test = 1;
      int SPACE = 0;
      int c = 0;
      int i = 0;
      int a = 0;
      char test1[BUF_SIZE];
      
      char test2[BUF_SIZE];

      while (test > 0)
	{
	  if (test = (read(0, test1, 1) != 0))
	    {
	      test2[c] = test1[0];
	      c++;
	      a = strlen(test2);

	      if (test1[0] == ' ' || test1[0] == '\n')
		{
		  if (test2[a] == ' ')
		    {
		      i++;
		    }
		  else
		    {
		      SPACE++;
		    }
		}
	    }
	}
      if (cflag1 == 1 || cflag2 == 1)
	{
	  printf("Number of Characters: ");
	  printf("%d\n", c);
	}
      if (wflag1 == 1 || wflag2 == 1)
	{
	printf("Number of Words: ");
	printf("%d\n", SPACE);
	}
      if (wcflag == 1 && wflag1 == 0 && cflag1 == 0)
	{
	  printf("Number of Characters: ");
	  printf("%d\n", c);
	  printf("Number of Words: ");
	  printf("%d\n", SPACE);
	}
    }
  return 1;
}


/*	  if (wflag1 == 1 || wflag2 == 1) // check for wflags
	    {
	      printf("Number of words: ");
	      printf("%d\n", SPACE);
	    }
      
	  if (cflag1 == 1 || cflag2 == 1) // check for cflags
	    {
	      printf("Number of characters: ");
	      printf("%d\n", c);
	    }
*/
