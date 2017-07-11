#include "stdlib.h"
#include <stdio.h>
#include "string.h"
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>



int main(int argc, char *argv[])
{
  int mat1row, mat1col, mat2row, mat2col;
  int i = 0;
  int j = 0;
  int id = 0;
  int id2 = 0;
  int a = 0;
  int b = 0;
  int q = 0;
  char line[LINE_MAX];
  int array1[1000];
  int array2[1000];
  int waitstat;
  char *token;
  char s[100];
  key_t key;

  extern char **environ;
  char *env_args[] = { "PATH=.", "USER=me", NULL};
  environ = env_args;
    
  while(fgets(line, LINE_MAX, stdin) != NULL)
    {
      if (line[0] == '\n')
	break;
      
      mat1col = 0;
      mat1row++;
      
      token = strtok(line, " \n");
      
      while(token != NULL)
	{
	  array1[id] = atoi(token);
	  id++;
	  mat1col++;
	  token = strtok(NULL, " \n");
	}
    }
  int mat1size = mat1col*mat1row*sizeof(int);
  int mat1id = shmget(IPC_PRIVATE, mat1size, IPC_CREAT | 0666);
  if(mat1id == -1)
    {
      perror("Shmget fail");
      exit(1);
    }

  int *mat1 = shmat(mat1id, NULL, 0);
  if(mat1 == (int *) -1)
    {
      perror("Shmat error");
      exit(1);
    }

  for(a=0; a < mat1row*mat1col; a++)
    {
      mat1[a] = array1[a];
    }


  while(fgets(line, LINE_MAX, stdin) != NULL)
    {
      mat2col = 0;
      mat2row++;

      token = strtok(line, " \n");

      while(token != NULL)
        {
	  array2[id2] = atoi(token);
	  id2++;
	  mat2col++;
	  token = strtok(NULL, " \n");
        }
    }
  int mat2size = mat2col*mat2row*sizeof(int);

  int mat2id = shmget(IPC_PRIVATE, mat2size , IPC_CREAT | 0666);

  if(mat2id == -1)
    {
      perror("shmget fail");
      exit(1);
    }

  int *mat2 = shmat(mat2id, NULL, 0);
    
  if(mat2 == (int *) -1)
    {
      perror("Shmat error");
      exit(1);
    }

  for(a=0; a < mat2row*mat2col; a++)
    {
      mat2[a] = array2[a];
    }
      
  if (mat1col != mat2row)
    {
      fprintf(stderr, "Cannot Multiply Given Matrices.\n");
      exit(1);
    }
  pid_t pid;
  pid_t cpid[mat1row*mat2col];
  
  int shmid = shmget(IPC_PRIVATE, mat1row*mat2col*sizeof(int), IPC_CREAT | 0666);
  int *shmarray = (int *)shmat(shmid,  NULL, 0);
  if(shmarray == (int *) -1)
    {
      perror("shmat error");
      exit(1);
    }
  int matrixsize = mat1row*mat2col;

  for(a=0; a < matrixsize; a++)
    {
      int fd[2];
      pipe(fd);
      pid = fork();
      if(pid == 0)
	{
	  close(fd[1]);
	  char *descriptor = calloc(sizeof(int)*8+1, 1);
	  sprintf(descriptor, "%i", fd[0]);
	  char *cmd = "multiply";
	  char *argm[3] ={"multiply", descriptor, NULL};
	  execvp(cmd, argm);
	  printf("exec error: abort\n");
	  exit(1);
	}
      cpid[a] = pid;
      close(fd[0]);
      dprintf(fd[1], "%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n", i, j,mat1col, mat2col, mat2row, mat1id, mat2id, shmid);
      close(fd[1]);
      j++;
      if( j == mat2col)
	{
	  i++;
	  j = 0;
	}

    }
  pid_t wa;
  for(b=0; b < mat1row*mat2col; b++)
    {
      if(wa =  waitpid(cpid[b], &waitstat, 0) == -1)
	{
	  perror("Wait error");
	  exit(1);
	}
    }

  for(i = 0;i < mat1row; i++)
    {
      for (j = 0;j < mat2col; j++)
	{ 
	  printf("%d ", shmarray[i*mat2col+j]);
	}
      printf("\n");
    }
  shmdt(mat1);
  shmdt(mat2);
  shmdt(shmarray);
  return 0;
}

