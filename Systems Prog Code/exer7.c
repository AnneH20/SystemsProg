/* 
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Emacs
Program Description: Program transfers string data entered in the parent
to the child over a pipe.
*/

#include "headers.h"
#define BUF_SIZE 1024
int fd[2];
char val[BUF_SIZE];
int main(int argc, char *argv[])
{
  pipe(fd);
  if (pipe(fd) == -1)
    {
      perror("pipe");
      exit(EXIT_FAILURE);
    }
  if (fork() != 0)
    {
      char *string = "Please enter text at the parent: ";
      write(STDOUT_FILENO, string, strlen(string));
      read(STDIN_FILENO, &val, BUF_SIZE);
      // scanf("%s", &val);

      if (write(fd[1], &val, sizeof(val)) == -1) // write to write end
	{
	  perror("write");
	  exit(EXIT_FAILURE);
	}
      printf("success: write\n");
      for (int i = 0; i < sizeof(val); i++)
	{
	  if (val[i] == '\n')
	    {
	      val[i] = '\0';
	    }
	}
      printf("\nparent: %s\n", val);
      close(fd[1]); // close write end
    }
  else
    {
      close(fd[1]); // close write end
      if (read(fd[0], &val, sizeof(val)) == -1) // read to read end
	{
	  perror("read");
	  exit(EXIT_FAILURE);
	}
      for (int i = 0; i < sizeof(val); i++)
	{
	  if (val[i] == '\n')
	    {
	      val[i] = '\0';
	    }
	}
      close(fd[0]); // close read end
      printf("child: %s\n", val);
  }
  return 0;
}

