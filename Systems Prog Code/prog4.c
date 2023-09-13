/*
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Complier Used: Emacs
Program Description: Write a program that is capable of playing tic-tac-toe against
any of your classmates tic-tac-toe programs using FIFOs, semaphores, and shared memory.
*/
 
#include "hdrs.h"
#define BUF_SIZE 1024

// initialize everything
struct mem
{
  int turn;
  int board[3][3];
};

union semun
{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *_buf;
};

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

// intConvert function
char intConvert(int i)
{
  switch(i)
    {
    case -1:
      return 'X'; // returns X on -1
    case 0:
      return ' '; // returns space on 0
    case 1:
      return 'O'; // returns O on 1
    }
}

// displayBoard funtion
void displayBoard(struct mem *g)
{
  printf(" %c | %c | %c\n", intConvert(g->board[0][0]), intConvert(g->board[0][1]), intConvert(g->board[0][2]));
  printf("-------------\n");
  printf(" %c | %c | %c\n", intConvert(g->board[1][0]), intConvert(g->board[1][1]), intConvert(g->board[1][2]));
  printf("-------------\n");
  printf(" %c | %c | %c\n", intConvert(g->board[2][0]), intConvert(g->board[2][1]), intConvert(g->board[2][2]));
  printf("-------------\n");
  printf("\n");
}
      
struct mem *makeMove(struct mem *g, int player)
{
  int range, i, j;
  srand(time(NULL));
  int counter = 0;
  int xflag = -1;
  switch(player)
  {
  case 1: // player 1
    if (g->turn == 0)
      {
	g->board[1][1] = -1;
	return g;
      }
    else
      {
	for (int i = 0; i < 3; i++) // checks horizontal win
	  {
	    if (g->board[i][0] == 1)
	      {
		counter++;
	      }
	    if (g->board[i][1] == 1)
	      {
		counter++;
	      }
	    if (g->board[i][2] == 1)
	      {
		counter++;
	      }
	    if (counter == 2)
	      {
		if (g->board[i][0] == 0)
		  {
		    g->board[i][0] = -1;
		    return g;
		  }
		if (g->board[i][1] == 0)
		  {
		    g->board[i][1] = -1;
		    return g;
		  }
		if (g->board[i][2] == 0)
		  {
		    g->board[i][2] = -1;
		    return g;
		  }
	      }
	    counter = 0;
	  }
	counter = 0;

	for (int i = 0; i < 3; i++) // checks vertical wins
	  {
	    if (g->board[0][i] == 1)
	      {
		counter++;
	      }
	    if (g->board[1][i] == 1)
	      {
		counter++;
	      }
	    if (g->board[2][i] == 1)
	      {
		counter++;
	      }
	    if (counter == 2)
	      {
		if (g->board[0][i] == 0)
		  {
		    g->board[0][i] = -1;
		    return g;
		  }
		if (g->board[1][i] == 0)
		  {
		    g->board[1][i] = -1;
		    return g;
		  }
		if (g->board[2][i] == 0)
		  {
		    g->board[2][i] = -1;
		    return g;
		  }
	      }
	    counter = 0;
	  }
	counter = 0;
	
	for (i = 0; i < 3; i++)
	  {
	    for (j = 0; j < 3; j++)
	      {
		if (g->board[i][j] == 0)
		  {
		    g->board[i][j] = -1;
		    return g;
		  }
	      }
	  }
      }

  case 2: // player 2
    if (g->turn == 0) // 1st turn
      {
	range = (rand() % (100 - 1 + 1) + 1);
	if (range < 25 && range > 0)
	  {
	    g->board[0][0] = 1;
	    return g;
	  }
	if (range < 50 && range > 24)
	  {
	    g->board[0][2] = 1;
	    return g;
	  }
	if (range < 75 && range > 49)
	  {
	    g->board[2][0] = 1;
	    return g;
	  }
	if (range < 101 && range > 74)
	  {
	    g->board[2][2] = 1;
	    return g;
	  }
      }
    else
      {
	if (g->board[0][0] == -1) // checks diagonal wins
	  {
	    if (g->board[2][2] == 0)
	      {
		g->board[2][2] = 1;
		return g;
	      }

	  }
	if (g->board[2][2] == -1)
	  {
	    if (g->board[0][0] == 0)
	      {
		g->board[0][0] = 1;
		return g;
	      }
	  }
	if (g->board[0][2] == -1)
	  {
	    if (g->board[2][0] == 0)
	      {
		g->board[2][0] = 1;
		return g;
	      }
	  }
	if (g->board[2][0] == -1)
	  {
	    if (g->board[0][2] == 0)
	      {
	        g->board[0][2] = 1;
	        return g;
	      }
	  }
	  
	for (int i = 0; i < 3; i++) // checks horizontal win
	  {
	    if (g->board[i][0] == -1)
	      {
		counter++;
	      }
	    if (g->board[i][1] == -1)
	      {
		counter++;
	      }
	    if (g->board[i][2] == -1)
	      {
		counter++;
	      }
	    if (counter == 2)
	      {
		if (g->board[i][0] == 0)
		  {
		    g->board[i][0] = 1;
		    return g;
		  }
		if (g->board[i][1] == 0)
		  {
		    g->board[i][1] = 1;
		    return g;
		  }
		if (g->board[i][2] == 0)
		  {
		    g->board[i][2] = 1;
		    return g;
		  }
	      }
	    counter = 0;
	  }

	for (int i = 0; i < 3; i++) // checks vertical wins
	  {
	    if (g->board[0][i] == -1)
	      {
		counter++;
	      }
	    if (g->board[1][i] == -1)
	      {
		counter++;
	      }
	    if (g->board[2][i] == -1)
	      {
		counter++;
	      }
	    if (counter == 2)
	      {
		if (g->board[0][i] == 0)
		  {
		    g->board[0][i] = 1;
		    return g;
		  }
		if (g->board[1][i] == 0)
		  {
		    g->board[1][i] = 1;
		    return g;
		  }
		if (g->board[2][i] == 0)
		  {
		    g->board[2][i] = 1;
		    return g;
		  }
	      }
	    counter = 0;
	  }
	counter = 0;
	
	for (i = 0; i < 3; i++)
	  {
	    for (j = 0; j < 3; j++)
	      {
		if (g->board[i][j] == 0)
		  {
		    g->board[i][j] = 1;
		    return g;
		  }
	      }
	  }
      }
  }
}

int checkWin(struct mem *g)
{
  // int counterP = 0;
  // int counterC = 0;
  int count = 0;
  
  // conditions to win
  int winCond[8][3] = {{g->board[0][0], g->board[0][1], g->board[0][2]},
		   {g->board[1][0], g->board[1][1], g->board[1][2]},
		   {g->board[2][0], g->board[2][1], g->board[2][2]},
		   {g->board[0][0], g->board[1][0], g->board[2][0]},
		   {g->board[0][1], g->board[1][1], g->board[2][1]},
		   {g->board[0][2], g->board[1][2], g->board[2][2]},
		   {g->board[0][0], g->board[1][1], g->board[2][2]},
		   {g->board[0][2], g->board[1][1], g->board[2][0]}};
  // win check
  for (int i = 0; i < 8; i++)
    {
      if (winCond[i][0] == -1 && winCond[i][1] == -1 && winCond[i][2] == -1)
	{
	  printf("Winner: X\n");
	}
      if (winCond[i][0] == 1 && winCond[i][1] == 1 && winCond[i][2] == 1)
	{
	  printf("Winner: O\n");
	}
    }
  for (int i = 0; i < 3; i++) // check for no plays left
    {
      for (int x = 0; x < 3; x++)
	{
	  if (g->board[i][x] != 0)
	    {
	      count++;
	    }
	}
    }
  if (count == 9) // if all spaces full
    {
      printf("Board complete!\n");
      return -1;
    }
  return 0;
}

// check for errors function
void assertError(int error, const char *str)
{
  if (error == -1)
    {
      perror("str");
      exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
  int low = 100000, high = 999999, count = 2;
  char *arg = argv[1];
  char *myfifo = "./xoSync"; // name of FIFO
  // to select which player, type ./prog4 # of player
  // ex: ./prog4 1 or ./prog4 2
  int player = atoi(arg); // decides which player to run
  int range[2], semID, sharID, fd, winCheck, tmp1, tmp2;
  char buf[1024];
  char buf2[1024];
  char buf3[1024];
  struct mem *smap;
  key_t sharMem, sema;
  union semun sem;
  struct mem tmp = {0, {0}};

  if (player != 1 && player != 2)
    {
      printf("Wrong player selected. Select 1 or 2\n");
      exit(EXIT_FAILURE);
    }
  if (player == 1 || player == 2)
    {
      printf("You are Player %d\n", player);
    }

  switch (player) // code for each player's game
    {
    case 1: // player 1
      printf("Player 1\n");
      fd = mkfifo(myfifo, 0644); // make fifo
      srand(time(NULL));
      for (int i = 0; i < count; i++) // create number
	{
	  range[i] = (rand() % (high - low + 1) + low);
	}
      if (range[0] == range[1])
	{
	  range[1] = range[1] + 1;
	}

      sharMem = ftok("./xoSync", range[0]);
      sema = ftok("./xoSync", range[1]);
      if ((sharID = shmget(sharMem, sizeof(struct mem), IPC_CREAT | 0644)) == -1)
	{
	  perror("shmget");
	  exit(EXIT_FAILURE);
	}

      if ((semID = semget(sema, 2, IPC_CREAT | 0644)) == -1)
	{
	  perror("semget");
	  exit(EXIT_FAILURE);
	}
      // initialize semaphores
      assertError(initSemAvailable(semID, 0), "semavail");
      assertError(initSemInUse(semID, 1), "semuse");

      // attatch shared mem
      smap = (struct mem *) shmat(sharID, NULL, 0);
      smap->turn = tmp.turn;
      for (int i = 0; i < 3; i++)
	{
	  for (int j = 0; j < 3; j++)
	    {
	      smap->board[i][j] = 0;
	    }
	}
      if (smap == (void *) - 1)
	{
	  perror("smap");
	}

      // FIFO Open
      if ((fd = open(myfifo, O_WRONLY, 0644)) == -1)
	{
	  perror("open");
	  exit(EXIT_FAILURE);
	}

      // Write range
      sprintf(buf, "%d", range[0]);
      write(fd, buf, 7);
      sprintf(buf, "%d", range[1]);
      write(fd, buf, 7);
      close(fd);

      while (smap->turn != -1)
	{
	  assertError(reserveSem(semID, 0), "reserve");
	  displayBoard(smap);
	  smap = makeMove(smap, 1);
	  displayBoard(smap);
	  if ((winCheck = checkWin(smap)) == -1)
	    {
	      smap->turn = -1;
	    }
	  assertError(releaseSem(semID, 1), "release");
	}

      // Open FIFO for write
      if ((fd = open(myfifo, O_WRONLY, 0644)) == -1)
        {
          perror("open");
          exit(EXIT_FAILURE);
        }

      // close FIFO
      close(fd);

      // Detach shared mem
      shmdt(smap);
      
      semctl(semID, 0, IPC_RMID);
      shmctl(sharID, 0, IPC_RMID);
      exit(EXIT_SUCCESS);
    case 2: // player 2
      printf("Player 2\n");
      fd = mkfifo(myfifo, 0644); // make FIFO or continue

      if ((fd = open(myfifo, O_RDONLY, 0644)) == -1)
        {
          perror("open");
          exit(EXIT_FAILURE);
        }
      read(fd, buf2, 7);
      read(fd, buf3, 7);

      for (int i = 0; i < 12; i++)
	{
	  if (buf2[i] == '\n')
	    {
	      buf2[i] = '\0';
	      break;
	    }
	}
      for (int i = 0; i < 12; i++)
	{
	  if (buf3[i] == '\n')
	    {
	      buf3[i] = '\0';
	      break;
	    }
	}
      
      tmp1 = atoi(buf2);
      tmp2 = atoi(buf3);

      // close FIFO
      close(fd);

      // Generate System V keys
      sharMem = ftok("./xoSync", tmp1);
      sema = ftok("./xoSync", tmp2);

      
      // retrieve sharmem and semephore
      assertError(semID = semget(sema, 0, 0), "semget");
      assertError(sharID = shmget(sharMem, 0,0), "shmget");

      // attach shared memory
      smap = (struct mem *) shmat(sharID, NULL, 0);

      // Gameplay loop
      while(1)
	{
	  reserveSem(semID, 1);
	  displayBoard(smap);

	  if (smap->turn == -1)
	    {
	      break;
	    }

	  smap = makeMove(smap, 2);
	  displayBoard(smap);
	  smap->turn = smap->turn + 1;
	  releaseSem(semID, 0);
	}
	checkWin(smap);

      // Open FIFO for read
      if ((fd = open(myfifo, O_RDONLY, 0644)) == -1)
        {
          perror("open");
          exit(EXIT_FAILURE);
        }
      
      // close FIFO
      close(fd);

      // detach shared mem
      shmdt(smap);
      exit(EXIT_SUCCESS);
    }

  // if all else fails
  printf("Darn. It broke.\n");
  exit(EXIT_SUCCESS);
}
