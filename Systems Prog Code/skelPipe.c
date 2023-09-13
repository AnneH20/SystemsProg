/* 
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Emacs
Program Description: Writes a concurrent process porgram that uses a pipe to display the contents of a file to STDOUT
*/

/* Problem 1 -- List the include files you need for this program */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#define BUF_SIZE 1024

/* Problem 2 -- Write or more functions necessary to make the program work. At minmum, you will need function main */
// Error Function
void assertError(int val, const char *str)
{
  if (val == -1)
    {
      perror(str);
      exit(EXIT_FAILURE);
    }
}
// Main Function
int main(int argc, char *argv[])
{
  int file, fd[2];
  pid_t pID;
  char buf[BUF_SIZE];
  char *filename = argv[1];

  if (argc != 2)
    {
      printf("No file specified\n");
      exit(EXIT_FAILURE);
    }
  if (argc == 2)
    {
      if (file = (open(filename, 0644)) == -1)
	{
	  printf("File not found\n");
	  exit(EXIT_FAILURE);
	}
      close(file);
    }

  assertError(pipe(fd), "create pipe failure");

  switch(pID = fork())
    {
    case -1:
      assertError(-1, "fork");
      
    case 0: // child process
      close(fd[0]); // close read end
      assertError(dup2(fd[1], STDOUT_FILENO), "dup2");
      char *args[3] = {"cat", argv[1], NULL}; // cat
      assertError(execvp(args[0], args), "execvp");  // exec
      close(fd[1]); // close write end
      exit(EXIT_SUCCESS);
      
    default: // parent process
      close(fd[1]); // close write end
      int j;
      assertError(j = read(fd[0], buf, BUF_SIZE), "read");
      buf[j] = '\0';
      assertError(write(STDOUT_FILENO, buf, strlen(buf)), "write");
      //printf("%s\n", buf);
      close(fd[0]); // close read end
      exit(EXIT_SUCCESS);
    }
}
