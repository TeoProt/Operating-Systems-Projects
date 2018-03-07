#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define SIZE 128

void sort(int *numbers, int length);
char *remove_file_extension(char *mystr);


int main(int argc, char *argv[])
{
	FILE *fp;
	char path[1000];

	char command[70];
	int len = snprintf(command, sizeof(command), "find %s -name \"*.bin\" | wc -l", argv[1]);	// Setting up correctly the command to pass it at popen() function

	fp = popen(command, "r");
	while(fgets(path, 1000, fp) != NULL){}
	
	int n = atoi(path);

	pid_t pid;
	char files[1000];

	char com[70];
	int l = snprintf(com, sizeof(com), "find %s -name \"*.bin\"", argv[1]);
	FILE *fptr = popen(com, "r");
//	FILE *fptr = popen("find . -name \"*.bin\"", "r");
	
	int i = 0;
	printf("Parent pid = %d\n", getpid());	

	for(int w = 0; w<n; w++)
	{	
		if( (fgets(files, 1000, fptr)) != NULL)
		{
			pid = fork();
			files[strcspn(files, "\n")] = '\0';
			if(pid == 0)
			{
				FILE *ptr;
				if ( (ptr = fopen(files, "r") ) == NULL)
				{
					fprintf(stderr,"Error opening file\n");
					exit(1);
				}
				printf("In child procces (pid = %d)\n", getpid());
				int *numbers = (int *)malloc(SIZE * sizeof(int));
				int j = 0;
				int size = SIZE;
	
				while( !feof(ptr))
				{
					fscanf(ptr, "%d", &numbers[j]);
					j++;
					if ( j > SIZE)
					{
						int *numbers_reallocated = (int *)realloc(numbers, 512*sizeof(int));
						if (numbers_reallocated)
							numbers = numbers_reallocated;
					}
				}
				j -= 1;
				sort(numbers, j);
				char *file_name = remove_file_extension(files);
				strcat(file_name, "_sorted.bin");
				FILE *f = fopen(file_name, "w");
				for(int k=0; k<j; k++)
					fprintf(f, "%d\n", numbers[k]);
				exit(0);
			}
			else if ( pid < 0)
			{
				fprintf(stderr, "errof forking\n");
				exit(1);
			}
		}

		pid_t wpid;
		int status = 0;
		while ( (wpid = wait(&status)) > 0)
		{
			printf("Exit status of %d was %d\n", (int)wpid, status);
		}
	}
	return 0;
}

void sort(int *numbers, int length)
{
	/* Function to sort the numbers in ascending order. The classic bubble-sort algorithm */
	int temp = 0;
	for (int i=0; i<length; i++)
	{
		for(int j=i; j<length; j++)
		{
			if(numbers[i] > numbers[j])
			{
				temp = numbers[i];
				numbers[i] = numbers[j];
				numbers[j] = temp;
			}
		}
	}
}

char *remove_file_extension(char *mystr)
{
	/* Function to remove the .bin extension from the file name in order to be able to create our new file with a proper name. */
	char *retstr;
	char *lastdot;
	if (mystr == NULL)
		return NULL;
	if ( (retstr = malloc (strlen (mystr) + 1)) == NULL)
		return NULL;
    	strcpy (retstr, mystr);
    	lastdot = strrchr (retstr, '.');
    	if (lastdot != NULL)
		*lastdot = '\0';
    	return retstr;
}
