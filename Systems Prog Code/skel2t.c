/* Write a threaded program that does the following
   1. Prompts the user for the number of integers to enter
   2. Prompts the user for each integer and writes them into 
      a file named data.dat
   3. Determines how many integers are > 100
   4. Determines how many integers are < 100
   5. Outputs the total count for each group.

   The program should perform this task in the following way:
   Create a producer thread that:
   1. Prompts the user for the number of integers to enter
   2. Prompts the user for each integer and writes them into a file
      named data.dat
   Create a consumer thread that:
   1. Determines how many integers are > 100
   2. Outputs that value
   3. Sets that value to its exit value
   Create a consumer thread that:
   1. Determines how many integers are < 100
   2. Outputs that value
   3. Sets that value to its exit value
   Create a main thread that: 
   1. Creates threads 1 - 3
   2. Waits on the values of threads 2 and 3. 
   3. Outputs the values from threads 2 and 3.
 */

/* Include files */
#include <stdio.h>
#include <stdlib.h>
/* Problem 1. Add the necessary include files for this program */
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

/* 
   Global variables:
   We will need a mutex, a condition variable, and a predicate variable. 
   Recall that the predicate variable is the variable we use to determine
   whether data was available prior to our first call to pthread_cond_wait

 */

/* Problem 2. Declare the global variables for the predicate variable, 
   the mutex, and the condition variable. Do no forget to initialize 
   the mutex and the condition variable 
 */
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int pv;

union dtob
{
  int values;
  char bytes[4];
};
struct retval
{
  int val;
};

/* This is a convenience function for dealing with errors
   and threads
 */

void hndlError(int error, const char *str)
{
  if (error == 0) return;
  errno = error;
  perror(str);
  exit(EXIT_FAILURE);
}


/* Define the three thread start functions. 
   You can name them whatever you wish 
 */

/* Problem 3. Define and write the start function for thread 1 */
void *threadStart1(void *arg)
{
  // pthread_detach(threadStart1); tried to use this to detach thread 1 but it kept breaking my code so I commented it out in order to get my program to run
  union dtob value;
  int openFlags = O_CREAT | O_WRONLY| O_TRUNC;
  mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  char file[] = "data.dat";

  int fd = open(file, openFlags, filePerms);
  int numEntries;

  printf("How many integers would you like to enter? ");
  scanf("%d", &numEntries);
  printf("\n");

  pthread_mutex_lock(&mtx);
  
  int num;
  for (int i = 0; i < numEntries; i++)
    {
      printf("Enter value number %d: ", i + 1);
      scanf("%d", &num);
      printf("\n");
      value.values = num;
      if (write(fd, value.bytes, 4) != 4)
        {
	  perror("write");
	  exit(EXIT_FAILURE);
        }
    }
    close(fd);
    pv = 1;
    pthread_mutex_unlock(&mtx);
    pthread_cond_broadcast(&cond);
    pthread_exit(NULL);
}


/* Problem 4. Define and write the start function for thread 2 */
void *threadStart2(void *arg)
{
  int more100 = 0;
  struct retval *r = (struct retval*)malloc(sizeof(struct retval));
  pthread_mutex_lock(&mtx);

  while (pv == 0)
    {
      pthread_cond_wait(&cond, &mtx);
    }

  int openFlags = O_RDONLY;
  mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP |  S_IWGRP | S_IROTH | S_IWOTH;
  char file[] = "data.dat";
  int fd = open(file, openFlags, filePerms);

  union dtob *fileCont = malloc(4);
  pthread_mutex_unlock(&mtx);

  int readBytes = read(fd, fileCont, 4);
  while (readBytes == 4)
    {
      if (fileCont->values > 100)
        {
	  more100++;
        }
      readBytes = read(fd, fileCont, 4);
    }
  r->val = more100;
  close(fd);
  printf("Total larger: %d\n", more100);
  pthread_exit(r);
}


/* Problem 5. Define and write the start function for thread 3 */
void *threadStart3(void *arg)
{
  int less100 = 0;
  struct retval *r = (struct retval*)malloc(sizeof(struct retval));
  pthread_mutex_lock(&mtx);

  while (pv == 0)
    {
      pthread_cond_wait(&cond, &mtx);
    }

  int openFlags = O_RDONLY;
  mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  char file[] = "data.dat";
  int fd = open(file, openFlags, filePerms);

  union dtob *fileCont = malloc(4);
  pthread_mutex_unlock(&mtx);

  int readBytes = read(fd, fileCont, 4);
  while (readBytes == 4)
    {
      if (fileCont->values < 100)
        {
	  less100++;
        }
      readBytes = read(fd, fileCont, 4);
    }
  r->val = less100;
  close(fd);
  printf("Total smaller:  %d\n", less100);
  pthread_exit(r);
}


/* Problem 6: Define and write the function for the main thread */
int main(int argc, char *argv[])
{
  pthread_t t1, t2, t3;
  int s;

  s = pthread_create(&t2, NULL, threadStart2, NULL);
  if (s != 0)
    {
      hndlError(s, "pthread2_create");
    }

  s = pthread_create(&t3, NULL, threadStart3, NULL);
  if (s != 0)
    {
      hndlError(s, "pthread3_create");
    }

  s = pthread_create(&t1, NULL, threadStart1, NULL);
  if (s != 0)
    {
      hndlError(s, "pthread1_create");
    }

  s = pthread_join(t1, NULL);
  if (s != 0)
    {
      hndlError(s, "pthread_join");
    }

  void *rt3, *rt2;
  s = pthread_join(t2, &rt2);
  if (s != 0)
    {
      hndlError(s, "pthread2_join");
    }

  s = pthread_join(t3, &rt3);
  if (s != 0)
    {
      hndlError(s, "pthread3_join");
    }

  struct retval *r2, *r3;
  r2 = (struct retval*) rt2;
  r3 = (struct retval*) rt3;

  printf("\nthread2 returned: larger: %d\n", r2->val);
  printf("thread3 returned: smaller: %d\n", r3->val);
    
}

