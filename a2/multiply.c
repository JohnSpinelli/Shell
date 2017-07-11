#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
  int res = 0;
  int k;

  int pipeDescriptor = atoi(argv[1]);
  FILE *pipe = fdopen(pipeDescriptor, "r");
    
  int mat1col;
  int mat2col;
  int mat2row;
    
  int i;
  int j;
  int shmid;
  int mat1id;
  int mat2id;

  char *temp = calloc(25,1);
  fgets(temp, 25, pipe);
  i = atoi(temp);

  fgets(temp, 25, pipe);
  j = atoi(temp);

  fgets(temp, 25, pipe);
  mat1col = atoi(temp);

  fgets(temp, 25, pipe);
  mat2col = atoi(temp);

  fgets(temp, 25, pipe);
  mat2row = atoi(temp);

  fgets(temp, 25, pipe);
  mat1id = atoi(temp);
    
  fgets(temp, 25, pipe);
  mat2id = atoi(temp);
    
  fgets(temp, 25, pipe);
  shmid = atoi(temp);
    
  int *mat1 = (int *)shmat(mat1id, NULL, 0);
  int *mat2 = (int *)shmat(mat2id, NULL, 0);
  int *shmarray = (int *)shmat(shmid, NULL, 0);

  if(mat1 == (void *) -1 || mat2 == (void *) -1 || shmarray == (void *) -1)
    {
      perror("Error attaching shared memory");
      exit(1);
    }

  for(k = 0; k < mat2row; k++)
    {
      res += mat1[i*mat1col+k]*mat2[k*mat2col+j];

    }
  shmarray[i*mat2col+j] = res;
  shmdt(shmarray);
  shmdt(mat1);
  shmdt(mat2);
  return 0;
}
