/*
Student Name: Anne Marie Heiderbeicht
Student NetID: alh1310
Compiler Used: Emacs
Program Description:
Using Exercise 1, this program will report the fake flow device from flowData.dat. 
The program will print an alert message when it reads a value not within the range.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>

const char* filename = "flowData.dat"; // file to be opened; could technically be changed depending on the exercise 1 arguements

union dtob // union from exercise 1
{
  int value;
  char bytes[4];
};

int main (void)
{
  int fd = open(filename, O_RDONLY); // file descriptor to open file using open
  if (fd == -1)
    {
      perror("Failure to open"); // error message if failure to open
      exit(EXIT_FAILURE);
    }
  union dtob *fileCont = malloc(2); // allocate for 2 bytes
  int lastAlert = 0;
  
  while (read(fd, fileCont, 2) != 0) // read file using read and loops
    {
      printf("Current value: %d\n", fileCont->value); // current entry value to help check if program is converting to integer correctly
      if (551 > fileCont->value) // Alert <= 550 is low
	{
	  printf("Alert! Alert!\n");
	  printf("Pressure in the lines is low\n");
	  printf("The number of entries read since last alert is: %d\n", lastAlert); // data entries since last alert
	  lastAlert = 0;
	  printf("\n");
	}
      if (1449 < fileCont->value) // Alert >= 1450 is high
	{
	  printf("Alert! Alert!\n");
	  printf("Pressure in the lines is high\n");
	  printf("The number of entries read since last alert is: %d\n", lastAlert); // data entries since last alert
	  lastAlert = 0;
	  printf("\n");
	}
      if(949 < fileCont->value && fileCont->value < 1051) // between 950 and 1050
	{
	  lastAlert++; // not alerting entry so increment by 1 since last alert
	}
    }
  
  close(fd); // close file descriptor
  free(fileCont); // free 2 bytes for fileCont
  exit(EXIT_SUCCESS); // exit success
}
  
