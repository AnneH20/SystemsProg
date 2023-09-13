/* 
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Emacs
Program Description: Creates a process and uses an exec function
to run the myRand program
*/

#include "headers.h"

union dtob
{
  int value;
  char bytes[2];
};


int main(int argc, char *argv[])
{
  pid_t c;
  char file[] = "./myRand";
  char *args[] = {"myRand.c", NULL};
  char *envp[] = {NULL};
  char dat[50];
  char filename[50];
  char num[4];
  int status;
  int x;
  int count;
  int fd;
  int val;
  double sum, average;
  int openFlags = O_RDONLY;

  union dtob buf;
  
  c = fork(); // create child

  if (c == 0)
    {
      if (execve(file, args, envp) == -1) // exec function
	{
	  perror("exec");
	  exit(EXIT_FAILURE);
	}
    }
  else if (c < 0)
    {
      perror("fork");
      exit(EXIT_FAILURE);
    }
  else
    {
      printf("Parent!\n");
      
      waitpid(c, &status, 0); // wait for child to terminate
      x = WEXITSTATUS(status);
      // printf("X is: %d\n", x);
      
      sprintf(num, "%d", x); // int x to char

      //create "dataX.dat"
      strcpy(filename, "data");
      strcpy(dat, ".dat");
      strcat(filename, num);
      strcat(filename, dat);
      printf("%s\n", filename);
      //open "dataX.dat"
      fd = open(filename, openFlags, 0644);
      if (fd  == -1)
	{
	  perror("open");
	}
      printf("Values: ");
      while (read(fd, buf.bytes, 2) != 0)
	{
	  val = buf.value;
	  printf("%d ", val);
	  sum = sum + val;
	  count++;
	}
      printf("\nNumber of values read: %d\n", count);
      average = sum / 60;
      printf("Average: %f\n", average);
    }
  unlink(filename);
  exit(EXIT_SUCCESS);
}
