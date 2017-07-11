#include "stdlib.h"
#include <stdio.h>
#include "string.h"
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/* Worked on by John Spinelli, jspinell@csa2.bu.edu. Implementation for reading in matrices worked on by John Spinelli and Clayton Ezzell */


int main(int argc, char *argv[]) {
  
  /* Read in Array Values from standard input */

  int mat1row, mat1col;
  char line[LINE_MAX];
  int array1[1000];
  int i,j;
  int id = 0; 
  char *token;

  while(fgets(line, LINE_MAX, stdin) != NULL)
    {
      if (line[0] == '\n')
	{
	  break;
	}
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
  
  /* Initialize input matrix with array values */
 
  int inputMat[mat1row][mat1col];
  
  
  for (i = 0; i < mat1row; i++) {
    for (j = 0; j < mat1col; j++) {
      inputMat[i][j] = array1[i * mat1col + j];
    }
  }
  
  
  /* Initialize Transpose Matrix */

  int transMat[mat1col][mat1row];
  for (i = 0; i < mat1row; i++) {
    for (j = 0; j < mat1col; j++) {
      transMat[j][i] = inputMat[i][j];
    }
  }
  
  /* Print Transpose of Matrix */

  printf("The transpose of our input matrix is as follows:\n");
  for (i = 0; i < mat1col; i++) {
    for (j = 0; j < mat1row; j++) {
      printf("%d ", transMat[i][j]);
    }
    printf("\n");
  }
  printf("\n");
 

  return 0;

}
