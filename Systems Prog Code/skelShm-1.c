/* 
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Emacs
Program Description: Write a multiple concurrent process porgram that uses shared memory and semaphores to display "ascii modern art"
*/

/* Problem 3 -- List the include files you need for this program */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <inttypes.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>

union semun
{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *_buf;
};

// binary_sem.c
int bsUseSemUndo;
int bsRetryOnEintr;

int initSemAvailable(int semID, int semNum)
{
  union semun arg;
  arg.val = 1;
  return semctl(semID, semNum, SETVAL, arg);
}

int initSemInUse(int semID, int semNum)
{
  union semun arg;
  arg.val = 0;
  return semctl(semID, semNum, SETVAL, arg);
}

int reserveSem(int semID, int semNum)
{
  struct sembuf sops;
  sops.sem_num = semNum;
  sops.sem_op = -1;
  sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

  while (semop(semID, &sops, 1) == -1)
    {
      if (errno != EINTR || !bsRetryOnEintr)
	{
	  return -1;
	}
    }
  return 0;
}

int releaseSem(int semID, int semNum)
{
  struct sembuf sops;
  sops.sem_num = semNum;
  sops.sem_op = 1;
  sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;
  return semop(semID, &sops, 1);
}
// Error Function
void assertError(int val, const char *str)
{
  if (val == -1)
    {
      perror(str);
      exit(EXIT_FAILURE);
    }
}

/* Problem 4 -- remember to declare a structure that represents the data stored in the shared memory */
struct duo
{
  int number;
  char letter;
};

struct mem
{
  int count;
  struct duo arr[];
};

/* Problem 5 -- create a function to handle the code for the child. 
Be certain to pass this function the id values for both the semaphore and the shared memory segment */
void cProcess(int shar, int sem)
{
  struct mem *smap;
  int count, length, alpha;
  int low = 10, high = 20;
  char alphabet[26] = { 'a', 'b', 'c', 'd', 'e','f', 'g', 'h', 'i', 'j', 'k',
			'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			'w', 'x', 'y', 'z' };
  // attached shar mem
  smap = (struct mem *)shmat(shar, NULL, 0);
  // seed random num gen
  srand(time(NULL));
  // reserve child semaphore
  assertError(reserveSem(sem, 0), "reserve1");

  // generate num of blocks
  count = (rand() % (high - low + 1) + low);
  smap->count = count;

  // Write to shared memory
  for (int i = 0; i < count; i++)
    {
      // length
      length = (rand() % (10 - 2 + 1) + 2);
      smap->arr[i].number = length;
      // character
      alpha = (rand() % (25 - 0 + 1) + 0);
      smap->arr[i].letter = alphabet[alpha];
    }
  
  // Releasing parent semaphore & reserving child semaphore
  assertError(releaseSem(sem, 1), "release");
  assertError(reserveSem(sem, 0), "reserve2");
  
  // Detach from child process
  assertError(shmdt(smap), "detach");
  // Releasing parent semaphore
  assertError(releaseSem(sem, 1), "release");
}

/* Problem 6 -- create a function to handle the code for the parent.
   Be certain to pass this function the id values for both the semaphore and the shared memort segment */
void pProcess(int shar, int sem)
{
  struct mem *smap;
  int wid;
  int count = 0;
  int low = 10, high = 15;

  // attach mem
  smap = (struct mem *)shmat(shar, NULL, 0);
  // seed random num generator
  srand(time(NULL));
  // reserve parent semaphore
  assertError(reserveSem(sem, 1), "reserve3");
  // width generate
  wid = (rand() % (high - low + 1) + low);
  printf("Each line has %d characters\n\n", wid);
  // data in shared mem
  for (int i = 0; i < smap->count; i++)
    {
      for (int j = 0; j < smap->arr[i].number; j++)
	{
	  printf("%c", smap->arr[i].letter);
	  count++;
	  if (count == wid)
	    {
	      printf("\n");
	      count = 0;
	    }
	}
    }
  printf("\n");
  // release child sem
  assertError(releaseSem(sem, 0), "release");
  // reserve parent sem
   assertError(reserveSem(sem, 1), "reserve4");
  // detach shar mem
  assertError(shmdt(smap), "detach");
}

/* Problem 7 -- implement function main */
int main(int argc, char *argv[])
{
  int semID, sharID;
  union semun sem;
  pid_t c;
  assertError(semID = semget(IPC_PRIVATE, 2, IPC_CREAT | 0644), "semget"); // create semID wit semget()

  assertError(initSemAvailable(semID, 0), "semavail"); // setting child(0) as available
  assertError(initSemInUse(semID, 1), "semuse"); // setting parent(1) as in use

  // shar mem set
  assertError(sharID = shmget(IPC_PRIVATE, sizeof(struct mem), IPC_CREAT | 0644), "shmget"); // create shmID with shmget()
  
  c = fork(); // create child process

  switch(c)
    {
    case -1:
      assertError(c, "fork");
    case 0:
      cProcess(sharID, semID);
      exit(EXIT_SUCCESS);
    default: // parent process
      pProcess(sharID, semID);
    }
  // delete sem
  assertError(semctl(semID, 0, IPC_RMID), "delete sem");
  // delete shar mem
  assertError(shmctl(sharID, 0, IPC_RMID), "delete shar mem");

  exit(EXIT_SUCCESS);
}
