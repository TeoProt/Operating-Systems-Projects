/*
 * 
 * author: Teo Protoulis
 *
 * date:   30/12/2017
 *
 * Help sources: 1) www.stackoverflow.com
 * 		 2) www.tutorialspoint.com
 * 		 3) www.github.com
 *
 * A command line interpreter for both interactive or batch mode. Commands can be separated by ; or &&. It handles whitespace,
 * empty lines and empty commands. Doesn't include the built-in shell commands. Hope you like it!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/wait.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>

#define BUFFSIZE 512 

typedef char* string;

string get_string(void);                  //fuction prototypes
string *parse_line(string line);
int execution_interactive(string *arguments);
void execution(string *arguments);


bool CheckAlpha(string source)
{
	//printf("\n\n...CHECKING FOR ONLY SPACES...\n\n");
	char *i = source;
	while (*i != 0)
	{
		if (!isspace(*i))
			return true;
		i++;
	}
	return false;
}

char *trimwhitespace(char *str)
{
	  //printf("...trimming...\n");
	  char *end;

	  // Trim leading space
	  while(isspace((unsigned char)*str)) str++;

	  if(*str == 0)  // All spaces?
	  	return str;

	  // Trim trailing space
 	  end = str + strlen(str) - 1;
  	  while(end > str && isspace((unsigned char)*end)) end--;

 	  // Write new null terminator
  	  *(end+1) = 0;

	  return str;
}

int main(int argc, char **argv)
{
	bool function = true;
	char continue_function = 'n';	
	int flag = 0;
	pid_t pid, wpid;
	int stat, exec_status;
	string line;
	string *arguments = NULL;
	int status = 1;
	string line_1 = NULL;
			
	bool is_alpha = true;
	if (argc == 1)
	{
		printf("\nHello and welcome to my command line intepreter. You can execute many commands by separating with either ; or &&\n");
		printf("\nIf you want to execute many commands and then exit just follow the above pattern and include anywhere you want the command: quit.\n\n");
		do
		{
			do
			{						//do..while loop to check the corectness of the entering command
				printf("protoulis_7968>> ");
				line = get_string();
			}while(strcmp(line,"") == 0 || line[0] == ';');

			char *copy = strdup(line);
	
			if ( strstr(line, "quit"))
				status = 0;

			if ( (pid = fork() ) > 0)
			{
				while ( (wpid = wait(&stat)) > 0){}
					//printf("...waiting for child to finish execution...\n");
			}
			else if (pid == 0)
			{
				printf("\n");
				line_1 = strtok(line, "&&;\t\r\n\a");
				while(line_1 != NULL)
				{
					if( CheckAlpha(line_1) )
					{
						line_1 = trimwhitespace(line_1);
						if ( strcmp(line_1, "quit") == 0 )
						{
							line_1 = strtok(NULL, "&&;\t\r\n\a");
							if (line_1 == NULL)
								exit(0);
							if ( CheckAlpha(line_1) )
								line_1 = trimwhitespace(line_1);
							else
							{
								line_1 = strtok(NULL, "&&;\t\r\n\a");
								line_1 = trimwhitespace(line_1);
							}
						}
						arguments = parse_line(line_1);
						int exec_status = execution_interactive(arguments);
						line_1 = strtok(NULL, "&&;\t\r\n\a");
					}
					else
					{
						line_1 = strtok(NULL, "&&;\t\r\n\a");
						continue;
					}

				}
				exit(0);
			}
			else
			{
				fprintf(stderr, "Error forking.\n");
				exit(0);
			}
			free(line);
			free(line_1);
			free(arguments);
			printf("\n");
		}while(status);
	}
	else if (argc == 2)
	{
		char line[512];
		string *arguments = NULL;
		string line_1 = NULL;
		int flag = 0;
		size_t len = 0;
		ssize_t read = 0;
		bool is_alpha = true;
		
		FILE *fptr = fopen(argv[1], "r");
		if (fptr == NULL)
		{
			fprintf(stderr, "No such file: %s.\n", argv[1]);
			exit(EXIT_FAILURE);
		}
		if ( (pid = fork() ) > 0)
		{
			while ( (wpid = wait(&stat)) > 0){}
                                 //printf("...waiting for child to finish execution...\n");
			
			while (continue_function != 'Y' && continue_function != 'y')
			{
				fprintf(stdout, "\nExecution of the commands in the batch file completed. Do you want to continue using this command line intepreter? Y/N ");
				scanf(" %c", &continue_function);
				getchar();
				if ( (continue_function == 'Y' || continue_function == 'y') )
				{
					function = true;
					char action = 'i';
					fprintf(stdout, "Type i for interactive mode or b for batch mode: ");
					scanf(" %c", &action);
					getchar();
					if ( action == 'i' || action == 'I')
						execl("./sample","", NULL);
					else if (action == 'b' || action == 'B')
						execl("./sample", "sample", "batchfile", NULL);
					//execl("./sample", "sample", "batchfile", NULL);
					else
					{
						continue_function = 'n';
					}
				}
				else if (continue_function == 'N' || continue_function =='n')
				{
					fprintf(stdout, "Bye!\n");
					exit(0);
				}
				else
				{
					fprintf(stderr, "Wrong input.\n\n");
				}
			}
                }
                else if (pid == 0)
                {
			while (fgets(line, sizeof(line), fptr) != NULL)
			{
				line_1 = strtok(line, "&&;\t\r\n\a");
				while (line_1 != NULL)
				{
					if ( CheckAlpha(line_1) )
					{
						line_1 = trimwhitespace(line_1);
						if ( strcmp(line_1, "quit") == 0 )
                                        	{
                                                	line_1 = strtok(NULL, "&&;\t\r\n\a");
                                                	if (line_1 == NULL)
							{
								line_1 = strtok(NULL, "&&;\t\r\n\a");
								continue;
							}
                                                	if ( CheckAlpha(line_1) )
                                                        	line_1 = trimwhitespace(line_1);
							else
							{
								line_1 = strtok(NULL, "&&;\t\r\n\a");
								line_1 = trimwhitespace(line_1);
							}
						}
						printf("Executing: %s\n\n", line_1);
						arguments = parse_line(line_1);
						execution(arguments);
						printf("\n");
					}
					line_1 = strtok(NULL, "&&;\t\r\n\a");
				}
			}
		}
		else
		{
			fprintf(stderr, "Error forking.\n");
			exit(0);
		}
		free(line_1);
		free(arguments);
		fclose(fptr);
	}
	else
		printf("Wrong usage.\n");

	return 0;
}

string *parse_line(string line)
{
	//printf("We are parsing rigth now!");

	string saveptr;
	int position = 0;
	string *tokens = malloc(BUFFSIZE * sizeof(char*));
	string toks = malloc(sizeof(char*));
	if(!tokens)
	{
		fprintf(stderr, "allocation error\n");
		exit(EXIT_FAILURE);
	}
	toks = strtok_r(line," \t\r\n\a", &saveptr);	//tokenizing each command - looking for command's arguments
	while(toks != NULL)
	{
		tokens[position] = toks;
		position++;
		toks = strtok_r(NULL, " \t\r\n\a", &saveptr);
	}
	tokens[position] = NULL;
	return tokens;
}

void execution(string *arguments)	//function which executes each command using fork()
{
	if (!isalpha(arguments[0][0]))
		return;
	//printf("Command is: %s\n", arguments[0]);
	if (strcmp(arguments[0], "quit") == 0)
		exit(0);
	pid_t pid, wpid;
	int status;
	pid = fork();    //fork creates a process which is referred to as a child process and the calling process is the parent process
	if(pid == 0)     //child proccess
	{
		if(execvp(arguments[0], arguments) == -1)
			perror("lsh: wrong command");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)    //error forking
	{
		perror("error forking");
	}
	else		//parent process
	{
		do{
			wpid = waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}

string get_string(void)	//a nice function to get the whole string a user entered until he hit \n
{
	string buffer = NULL;
	unsigned int capacity = 0;
	unsigned int n = 0;
	int c;
	while((c = fgetc(stdin)) != '\n' && c != EOF)
	{
		if (n + 1 > capacity)
		{
			if (capacity == 0)
				capacity = 32;
			else if (capacity <= (UINT_MAX / 2))
				capacity *= 2;
			else
			{
				free(buffer);
				return NULL;
			}
			string temp = realloc(buffer, capacity * sizeof(char));
			if (temp == NULL)
			{
				free(buffer);
				return NULL;
			}
			buffer = temp;
		}
		buffer[n++] = c;
	}
	if (n == 0 && c == EOF)
		return NULL;
	string minimal = malloc((n + 1) * sizeof(char));
	strncpy(minimal, buffer, n);
	free(buffer);
	minimal[n] = '\0';
	return minimal;
}

int execution_interactive(string *arguments)	//function which executes each command using fork()
{
	pid_t pid, wpid;
	int status;
	pid = fork();
	if(pid == 0)     //child process
	{
		if(execvp(arguments[0], arguments) == -1)
		{
			perror("myshell: wrong command");
		}
			//exit(EXIT_FAILURE)
		exit(0);
	}
	else if (pid < 0)    //error forking
		perror("error forking");
	else				//Parent process
	{
		do{
			wpid = waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
		
	}
	return 0;
}
