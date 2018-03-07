#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define T 2  // indicates how many times the alarm will function

volatile int flag = T;

void function();
void on_alarm();
int main() 
{  
  signal(SIGALRM, on_alarm);
  alarm(5);
  while(flag){sleep(1);}  
  return 0;
}

void on_alarm(int sig)
{
	signal(SIGALRM, SIG_IGN);
	printf("\n");	
	function();
	--flag;
	alarm(5);
	signal(SIGALRM, on_alarm);
}

void function()
{
  int pd[2];
  int pipe1[2];
  int pipe2[2];
  pid_t pid, pid_2;
  char line[512];

  char *cmd1[] = { "ps", "hax", "-o", "user", 0 };
  char *cmd2[] = { "sort", 0 };
  char *cmd3[] = { "uniq",  "-c", 0 };

    if (pipe(pd) < 0 || pipe(pipe1) < 0 || pipe(pipe2) < 0)
	perror("Cannot create all pipes.");

    if ((pid = fork()) < 0)
	perror("Cannot fork");

    if(pid > 0) 
    {
	    dup2 (pd[1], 1);
	    close(pd[0]);
	    close(pd[1]);
	    close(pipe1[0]);
	    close(pipe1[1]);
	    execvp(cmd1[0], cmd1);
	    pid_t term;
	    int stat;
	    while( (term = wait(&stat)) > 0)
	    {
		    printf("Child terminated.\n");
	    }	    
    }
 
    if ( (pid_2 = fork() ) == 0)
    {
	    dup2(pd[0], 0);
	    dup2(pipe1[1], 1);
	    close(pd[0]);
	    close(pd[1]);
	    close(pipe1[0]);
	    close(pipe1[1]);
	    execvp(cmd2[0],cmd2);	  
    }
    else if (pid_2 < 0)
	perror("cannot fork.");

    pid_t pid_3;
    if ( (pid_3 == fork()) == 0)
    {
	    dup2(pipe1[0], 0);
	    dup2(pipe2[1], 1);
	    close(pd[0]);
	    close(pd[1]);
	    close(pipe1[0]);
	    close(pipe1[1]);
	    execvp(cmd3[0],cmd3);
    }
    else if (pid_3 < 0)
	perror("cannot fork.");
    if (pid == 0) 
    {	
	    FILE *fptr = fopen("users.log", "a+");
	    if (fptr == NULL)
		fprintf(stderr, "could not open file.\n");
	    int file_number = fileno(fptr); // getting the file descriptor of fptr	    
	    close(pd[1]);
	    close(pipe1[1]);
	    int n = read(pipe2[0], line, sizeof(line));
	    fprintf(fptr, "%.*s", n, line);
	    fprintf(fptr, "\n");
	    fclose(fptr);
     }	
}
