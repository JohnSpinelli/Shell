#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMD 5
#define MAX_CMD_LEN 100
#define MAX_PIPES 5

extern char **environ;

int child_executing;

/*struct command
 {
 const char **argv;
 };*/

void sigint_handler(int signo) {
  if (!child_executing) {
    exit(EXIT_FAILURE);
  }
}

struct sigaction sigchld_action = {
  .sa_handler = SIG_DFL,
    .sa_flags = SA_NOCLDWAIT
};

void child_handler(int signo) {
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    
  sigaction(SIGCHLD, &sigchld_action, NULL); /* prevent zombies */
    
  signal(SIGINT, sigint_handler);
    
  child_executing = 0;
    
  pid_t pid;
    
  /* add environment*/
  char *env_args[] = { "PATH=/bin:/usr/bin:/usr/local/bin/:." , "USER=me", NULL};
    
  environ = env_args;
    
  int a = 0;
    
  //char buf[1000];
  //char str[1000];
  char buf[MAX_CMD_LEN]; 
  char str[MAX_CMD_LEN];    

  char *commands[MAX_CMD];
    
  int semicolon [MAX_CMD]; /*store index of ';'s*/
  int pipes [MAX_PIPES+1]; /*store index of '|'s*/
    
  /*no more than 5 commands in sequence*/
    
  int ftest = 1;
    
  /*while(1){*/
  while(ftest==1){
        
    child_executing = 1;
        
    FILE *fptr;
        
    //printf("myshell> ");
    /*char ch;
        while(read(STDIN_FILENO, buf ,MAX_CMD_LEN)>0)
	{

	}
	printf("buf %s \n ",buf);
    */
    if(isatty(0)==1){
      printf("myshell1> ");
      fgets(buf,sizeof(buf),stdin);
    }

    else
      {
	fptr = fopen("input.txt", "r");
	int f;
	fseek(fptr, 0, SEEK_END);
	int inputsize = ftell(fptr);
	char str[inputsize];
	printf("inputsize %i \n",inputsize);
	fseek(fptr,0,SEEK_SET);
	for(f = 0; f < inputsize; f++)
	  {
	    fscanf(fptr, "%c", &str[f]);
	  }
	str[inputsize] = '\0';
	ftest=0;
      }

    //str = "ls";
    buf[sizeof(buf)] = '\0'; 
        
    /*get first non space charachter*/
    int i=0;
    while(isspace(buf[i]))
      {
	i++;
      }
    printf("first i %i \n", i); 
    /*get rest of string*/
    int j=0;
    while(buf[i]!=0)
      {
	str[j] = buf[i];
	i++;
	j++;
      }
        
    str[i-1] = '\0';

    /*get indexes of ";"*/
    j=0;
        
    semicolon[0]=-1;
    int count = 1;
        
       
    while(str[j]!=0)
      {
	//printf("str[j] %c \n",str[j]);
            
	if(str[j]==59) { /*59 is ; in ASCII*/
	  semicolon[count]=j;
	  count++;
	}
	j++;
      }
      

    semicolon[count]=j;
        
    /*check for pipes, get indexes, count of pipe cmnds '|'*/
        
    int p = 0;
        
    int pipeCount = 0;
        
    while(str[p]!=0)
      {
            
	if(str[p]==124) { /*124 is | in ASCII */
                
	  pipes[pipeCount+1]=p;
	  pipeCount++;
                
	}
	p++;
      }
        
    /*seperate into seperate commands - no pipes */
    if(pipeCount<=0){
            
      if(semicolon[1]==j && count ==1) {/*no semicolons -- 1 command */
	commands[0] = str;
	commands[0][j] = '\0';
      }
      else{
                
	commands[0] = (char*)malloc(semicolon[1]);
	strncpy(commands[0], str, semicolon[1]);
	commands[0][semicolon[1]] = '\0';
	int k;
	for(k = 1; k<count; k++) {
	  /*printf("semicolon[k+1]-semicolon[k] %i \n",semicolon[k+1]-semicolon[k]-1);*/
	  commands[k] = (char*)malloc(semicolon[k+1]-semicolon[k]);
	  strncpy(commands[k], str + semicolon[k]+1, semicolon[k+1]-semicolon[k]-1);
	  commands[k][semicolon[k+1]-semicolon[k]-1] = '\0';
                    
	}
      }
      if(count==1)
	semicolon[1] = strlen(commands[0]);
      
      i,j=0;
    }
    else {/*seperate commands based on pipe indexes*/
            
      pipes[0] = -1;
            
      pipes[pipeCount+1] = strlen(str)+1;
            
      commands[0] = (char*)malloc(pipes[1]-1);
      strncpy(commands[0], str, pipes[1]-1);
      commands[0][pipes[1]-1] = '\0';
            
            
      for(p=1; p < pipeCount+1; p++) {
                
	commands[p] = (char*)malloc(pipes[p+1]-pipes[p]-3);
	strncpy(commands[p], str + pipes[p]+2, pipes[p+1]-pipes[p]-3);
	commands[p][pipes[p+1]-pipes[p]-3] = '\0';
                
      }
            
    }
        
    int control[count];
    int ctrlIndex[count];
    int out, in, out1, err, outerr;
 
    /*check control charachteres in each command*/
    for(i=0;i<count; i++) {
            
      out = 0;
      in = 0;
      out1 = 0;
      err = 0;
      outerr = 0;
            
      for(j=0; j < strlen(commands[i]); j++) {
	if(commands[i][j]==60) { /*<*/
	  in++;
	  ctrlIndex[i]=j;
                    
	}
	else if(commands[i][j]==62) { /*>*/
                    
	  if(commands[i][j-1]==49)  { /*1>*/
	    out1++;
	    ctrlIndex[i]=j;
	    
	    
	  }
                    
	  else if(commands[i][j-1]==50) { /*2>*/
	    err++;
	    ctrlIndex[i]=j;
	  }
                    
	  else if(commands[i][j-1]==38) { /*&>*/
	    outerr++;
	    ctrlIndex[i]=j;
	  }
                    
	  else {
	    out++;
	    ctrlIndex[i]=j;
	    
	  }
	}
                
      }

      if(out>1 || in>1 || out1>1 || outerr>1 || err>1) {
	fprintf(stderr, "command %i is invalid. Too many control charachters \n", i);
	control[i]=0;
	ctrlIndex[i]=-1;
      }
      else if(in == 1) /*in*/
	control[i]=1;
      else if(out == 1) /* out*/
	control[i]=2;
      else if(out1 == 1) /*out1 1>*/
	control[i]=3;
      else if(outerr == 1) /*outerr &>*/
	control[i]=4;
      else if(err == 1) /*err 2>*/
	control[i]=5;
      else
	control[i]=0;
            
    }
    if(count == 1 && commands[0][strlen(commands[0])-1]==38) {
      ctrlIndex[0] = strlen(commands[0])-1; /* for cmnd & aka run in bckground */
      control[0] = 6;
      commands[0][strlen(commands[0])-2] = '\0';
    }
        
    /* control[i]==2 || control[i]==1 || control[i] ==5 || control[i] ==0*/
        
        
    /*printf("commands[0] %s \n", commands[0]);*/
        
    /*execute file output step
         need to make indexes universal
         i goes from 0 to count-1
         take i+1 semicolon
         i ctrlIndex
         i command*/
        
    int* status;
    i=0;
        
    if(pipeCount<=0) {
      for(i=0; i<count; i++) {
                
	if(control[i]==2 || control[i]==1 || control[i] ==5 || control[i] ==0 || control[i]==6 || control[i]==3 || control[i]==4)
	  {
                    
	    char outcmnd[100];
	    char filename[100];
                    
	    if(control[i]>0){
                        
	      strncpy(outcmnd, &commands[i][0], ctrlIndex[i]-1);
	      outcmnd[ctrlIndex[i]-1] = '\0';
                        
	      strncpy(filename, &commands[i][0] + ctrlIndex[i] + 2, semicolon[i+1] - ctrlIndex[i] + 2 );
	      filename[semicolon[i+1] - ctrlIndex[i] + 2] = '\0';
	    }
	    else if(control[i]==0) {
                        
	      strncpy(outcmnd, &commands[i][0], strlen(commands[i]));
	      outcmnd[strlen(commands[i])] = '\0';
                        
	    }
                    
	    char * args[10];
                    
	    printf("outcmnd %s \n", outcmnd);
                    
	    /* TOKENIZE INDIVIDUAL COMMANDS */
	    const char s[2] = " ";
	    char *token;
                    
	    token = strtok(outcmnd, s);
                    
	    args[0] = token;
                    
	    int tokdex = 1;
	    while( token != NULL )
	      {
		token = strtok(NULL, s);
		args[tokdex]=token;
		tokdex++;
	      }
                    
	    args[tokdex] = NULL;
                    
	    printf("args[0] %s \n", args[0]);
                    
	    int fd;
                    
	    fd = open(filename, O_RDWR);
                    
	    if ((pid = fork()) < 0){
                        
	      perror("fork");
	      exit(EXIT_FAILURE);
                        
	    }
                    
	    else if (pid == 0) {
                        
                        
	      signal(SIGINT, child_handler);
                        
                        
	      pid_t pid2;
                        
	      if ((pid2 = fork()) < 0){
                            
		perror("fork");
		exit(EXIT_FAILURE);
	      }
	      else if (pid2 == 0) {
		if (control[i]==2 || control[i]==3) {
		  close(1);
		  dup(fd);
		  printf( "In stdout! \n");
		}
		else if(control[i]==1){
		  close(0);
		  dup(fd);
		}
		else if(control[i]==5){
		  close(2);
		  dup(fd);
		  fprintf(stderr, "In standard error! \n");
		}
		else if(control[i]==4){
                                
		  dup2(fd,1);
		  dup2(fd,2);
                                
		  fprintf(stderr, "In standard error! \n");
		  printf("In standard out too! \n");
                                
		}
		else if(control[i]==6)
		  {
		    setpgid(0, 0); /* background */
                                
		  }
		if(execvp(args[0], &args[0])!=-1){
		}
	      }
	      else{
		waitpid(pid2,status,0);
		close(fd);
		exit(0);
	      }
	    }
	    else {
	      waitpid(pid,status,0);
	    }
	  }
      }
    }
    else if(pipeCount>=1) /*run pipe commands*/
      {
	char *pipeCommands[MAX_CMD][MAX_CMD];
            
	/* TOKENIZE INDIVIDUAL COMMANDS */
            
	for(p=0; p<pipeCount+1; p++) {
                
	  const char s[1] = " ";
	  char *token;
                
	  token = strtok(commands[p], s);
	  pipeCommands[p][0] = token;
                
	  int tokdex = 1;
	  while( token != NULL) {
                    
	    token = strtok(NULL, s);
	    pipeCommands[p][tokdex]=token;
	    tokdex++;
	  }
                
	  pipeCommands[p][tokdex] = NULL;
	}
            
            
            
	int i;
	pid_t pid;
            
	int in;
	int fd [2]; /*2 file descriptors*/
            
	in = 0;
            
	for(p=0; p<pipeCount; ++p) {
                
	  if (pipe(fd) == -1) {
	    perror("pipe");
	    exit(EXIT_FAILURE);
	  }
                
	  next_process(p, in, fd[1], pipeCommands);
	  close (fd [1]);
	  in = fd[0];
                
                
	}
	if (in != 0)
	  dup2 (in, 0);
            
	/* Execute the last stage with the current process. */
	/*execvp (cmd [i].argv [0], (char * const *)cmd [i].argv);*/
	execvp (pipeCommands[pipeCount][0], &pipeCommands[pipeCount][0]);
            
      }
        
        
    child_executing = 0;
        
        
    /*command is from 0 to ctrl charachter index -1
      file name is from ctrl character index + 1to semi colon index*/
        
  }
  return 0;
}

/*parse algorithm:
 1 getstr
 2 cut off spaces at tails
 3 search for ;s, separate into separate commands in array
 4 for command about to run: search for control signals
 -isolate exec command, control, and optionally output files*/


int next_process (int p, int in, int out, char *pipeCommands[MAX_CMD][MAX_CMD]) /*give each process its in and out*/
{
  pid_t pid;
    
  if ((pid = fork()) < 0){
    perror("fork");
    exit(EXIT_FAILURE);
  }
  else if (pid == 0) {
    if (in != 0){
      dup2(in, 0); /*each command readsd from stdin*/
      close(in);
    }
    if (out != 1) {
      dup2 (out, 1); /*eaech command writese to stdout*/
      close (out);
    }
    return execvp (pipeCommands[p][0], &pipeCommands[p][0]);
  }
  return pid;
}








