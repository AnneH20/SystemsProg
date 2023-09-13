/* 
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Emacs
Program Description: Generates a file that creates 60 random numbers
between 0 and 255
*/

#include "headers.h"
#define BUF_SIZE 1024

union dtob
{
  int value;
  char bytes[2];
};

int main(int argc, char *argv[])
{
  int x;
  int fd;
  int fds;
  int y;
  char filename[50];
  char dat[50];
  char num[4];
  
  int openFlags = O_RDWR | O_CREAT | O_TRUNC;
  
  union dtob buf;
  srand(time(NULL));
  int low = 0, high = 255, count = 60;
  x = (rand() % (high - low + 1) + low); // how to get X in "dataX.dat"

  sprintf(num,"%d", x);

  strcpy(filename, "data"); // create "dataX.dat"
  strcpy(dat, ".dat");
  strcat(filename, num);
  strcat(filename, dat);
  // printf("%s\n", filename);
  high = 100; // change to 100 to generate numbers in file
  fd = open(filename, openFlags, 0644); // open
  if (fd == -1)
    {
      perror("open");
      exit(EXIT_FAILURE);
    }
  for (int i = 0; i < count; i++)
    {
      int y;
      y = (rand() % (high - low + 1) + low);
      buf.value = y;
      fds = write(fd, buf.bytes, 2); // write
      if (fds == -1)
	{
	  perror("write");
	  exit(EXIT_FAILURE);
	}
    }
  close(fd);
  exit(x);
}
