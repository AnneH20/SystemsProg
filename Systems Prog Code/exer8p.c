/* 
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Emacs
Program Description: Write a set of programs that transfers string data entered in one program to the other program over a FIFO.
*/

#include "headers.h"
#define BUF_SIZE 1024

int main (int argc, char *argv[]) // producer part
{
  int fd;
  char *myfifo = "/tmp/myfifo";
  mkfifo(myfifo, 0666); // create fifo
  char arr1[BUF_SIZE];
  // printf("Please enter text at the producer: ");
  char *string = "Please enter text at the parent: ";
  write(STDOUT_FILENO, string, strlen(string));
  
  while(1)
    {
      fd = open(myfifo, O_WRONLY); // open for writing
      // scanf("%s", arr2);
      // fgets(arr2, 80, stdin);
      read(STDIN_FILENO, arr1, BUF_SIZE); // read input
      for (int i = 0; i < strlen(arr1); i++)
	{
	  if (arr1[i] == '\n')
	    {
	      arr1[i] = '\0';
	    }
	}
      write(fd, arr1, strlen(arr1) + 1); // write to fifo
      close(fd); // close
      printf("\nproducer: %s\n", arr1);
    }
}
