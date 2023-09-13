/*
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Emacs
Program Description:
Creates a program that creates a directory structure for all files associated
with the exercises, programs, and tests
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char  *argv[])
  {
    char *rootDir = "sysProg"; //root directory default
    char *dirs[] = {"test", "program", "exercise"}; //subdirectories
    char *dir, *dir1;
  
    if (argc == 2)
      {
	rootDir = argv[1];
      }
    mkdir(rootDir, 0777);
    
    for (int i = 0; i < 3; i++)
      {
	char *dir = malloc(strlen(rootDir) + strlen(dirs[i]) + 2);
	sprintf(dir, "%s/%s", rootDir, dirs[i]);
	printf("creating directory named: %s\n", dir); //if given a specific root directory, creates root directory in that name
	mkdir(dir, 0777);
	
	switch(i){ // used switch bc i am lazy
	case 0:
	  dir1 = (char *)malloc(strlen(dir) + strlen("/test1") + 2);
	  for (int i = 1; i <= 3; i++) //loop through to create 3 test folders within main test folder
	    {
	      sprintf(dir1, "%s/test%d", dir, i);
	      printf("creating directory named: %s\n", dir1);
	      mkdir(dir1, 0777);
	    }
	  free(dir1);
	  break;
	  
	case 1:
	  dir1 = (char *)malloc(strlen(dir) + strlen("/prog1") + 2);
	  for (int i = 1; i <= 5; i++) //loop through to create 5 program folders within main program folder
	    {
	      sprintf(dir1, "%s/prog%d", dir, i);
	      printf("creating directory named: %s\n", dir1);
	      mkdir(dir1, 0777);
	    }
	  free(dir1);
	  break;
	  
	case 2:
	   dir1 = (char *)malloc(strlen(dir) + strlen("/exer1") + 2);
	   for (int i = 1; i <= 10; i++) //loop through to create 10 exercise folders within main exercise folder
	    {
	      sprintf(dir1, "%s/exer%d", dir, i);
	      printf("creating directory named: %s\n", dir1);
	      mkdir(dir1, 0777);
	    }
	  free(dir1);
	  break;
	}
	free(dir);
      }
    return 0;
  }
	  
    
