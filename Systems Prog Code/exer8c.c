/* 
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Emacs
Program Description: Write a set of programs that transfers string data entered in one program to the other program over a FIFO.
*/

#include "headers.h"
#define BUF_SIZE 1024

int main (int argc, char *argv[]) // consumer part
{
  int fd1;
  char *myfifo = "/tmp/myfifo";
  mkfifo(myfifo, 0666);
  char str1[BUF_SIZE];
  while(1)
    {
      fd1 = open(myfifo, O_RDONLY); // open fifo for reading
      read(fd1, str1, BUF_SIZE); // read fifo
      printf("consumer: %s\n", str1); // output
      // write(STDOUT_FILENO, str1, strlen(str1));
      close(fd1);
    }
}
