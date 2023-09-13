/*
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Emacs
Program Description:
Create a program that generates are random set of numbers and
writes them to a file
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

#define BUF_SIZE 4

union dtob
{
  int value;
  char bytes[4];
};

int outBytes(const char *out, int count, int low, int mid, int high) // outBytes function (based on lecture videos posted on Canvas)
{
  int outputFd, openFlags, i;
  mode_t filePerms;
  int rng;
  double offset;
  int val;
  union dtob outBytes;

  openFlags = O_CREAT | O_WRONLY | O_TRUNC;
  filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

  outputFd = open(out, openFlags, filePerms);

  if (outputFd == -1)
    {
      perror("Open to write");
      return -1;
    }

  for (i = 0; i < count; i++) // creates range value, offset value, lower range, mid range, and upper range
    {
      rng = rand() % 100;
      offset = (rand() % 1000 - 500)/1000.0;
      if (rng < 15)
        {
          val = (int) ((low + offset) * 100); // lower
        }
      else if (rng > 85)
        {
          val = (int) ((high + offset) * 100); // upper
        }
      else
        {
          val = (int) ((mid + offset) * 100); // middle
        }
      
      printf("%d\n", val); // to see the integers generated
      
      outBytes.value = val;
      if (write(outputFd, outBytes.bytes, 4) != 4)
        {
          perror("Write");
          return -1;
        }
   }
  close (outputFd);
  return 0;
}

char *copyStr(const char *val) // based function off of a references from https://bytes.com/topic/c/answers/220941-strdup) did not copy from website, just got an idea of how to allocate memory for the created file
{
  int length = strlen(val);
  char *outputStr = (char *) calloc(length + 1, sizeof(char)); // allocate space
  if (outputStr == NULL)
    {
      perror("Calloc"); // error message
      exit(EXIT_FAILURE);
    }
  strcpy(outputStr, val);
  return outputStr;
}


int main (int argc, char *argv[]) // argc is the number of arguements given and argv are the variables; argv[0] is the executable; argv[1] is the user picked file name; argv[2] is the number; argv[3][4][5] are the low, mid, and high values
{
  char *outputFile = NULL;
  int cnt = 60; // default number generator
  int low =5; // default low value
  int mid = 10; // default mid value
  int high = 15; // default high value

  srand(time(NULL));
  if (argc != 1 && argc !=3 && argc !=6)
    {
      perror("Wrong call to createFile"); // catch error
      exit(EXIT_FAILURE);
    }
  if (argc == 1)
    {
      outputFile = copyStr("flowData.dat"); // default file name
      printf("Success. Check binary file.\n");
    }
  if (argc >  2)
    {
      outputFile = copyStr(argv[1]); // takes in 2nd arguement as file name
      cnt = atoi(argv[2]); // takes in 3rd arguement as numbers to generate
      printf("Success. Check binary file.\n");
    }
  if (argc >  5)
    {
      low = atoi(argv[3]); // takes in 4th arguement as low value
      mid = atoi(argv[4]); // takes in 5th arguement as mid value
      high = atoi(argv[5]); // takes in 6th arguement as high value
    }

  if (outBytes(outputFile, cnt, low, mid, high) == -1)
    {
      perror("Failed to write the file."); // catch error
      exit(EXIT_FAILURE);
    }
  exit(EXIT_SUCCESS);
}

