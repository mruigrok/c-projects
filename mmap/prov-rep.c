#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>

/*---Global Variables----*/

int sem;
key_t key;

struct sembuf sem_inc = {
	.sem_num = 0,
	.sem_op = 1,
	.sem_flg = 0
};

struct sembuf sem_dec = {
	.sem_num = 0,
	.sem_op = -1,
	.sem_flg = 0
};

/*--------Function declaration--------*/

void sem_wait();
void sem_signal();
void print_file_contents(char[], int);
int contains_val(int[], int, int);
void child_process(char *, int);
void parent_process(char *, int, pid_t);

/*-----------Main program------------*/

int main(int argc, char *argv[])
{
	/*Initialize or locate a semaphore, then open the 
	file and map it*/

	key = ftok("./res.txt", 1);
	sem = semget(key, 1, 0);

	if(sem == -1)
	{
		printf("Creating semaphore...\n");
		sem = semget(key, 1, IPC_CREAT);
	}
	else
	{
		printf("Found old semaphore....\n");
	}

	printf("Semaphore ID: %d\n", sem );

	
	char *filename = argv[1];
	int fd = open(filename, O_RDWR, S_IRUSR | S_IWUSR);
	struct stat sb;

	if(fstat(fd, &sb) == -1)
	{
		perror("couldn't get the file size");
	}

	char *file_contents = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	/*Go through the file and find the resources that are available to add
	or allocate and change the global variables resource_arr and size. These 
	resources will remain constant throught the execution but the amount of 
	them that are available will vary*/

	pid_t pid;

	if((pid = fork()) < 0)
	{
		perror("didn't create the processes");
		exit(1);
	}

	if(pid == 0)
	{
		//printf("Child process ID: %d\n", getpid());
		child_process(file_contents, sb.st_size);
	}
	else{
		//printf("Parent process ID: %d\n", getpid());
		parent_process(file_contents, sb.st_size, pid);
	}


}//end of main


/*----------Function Definition---------*/

void sem_wait()
{
	semop(sem, &sem_dec, 1);
}

void sem_signal()
{
	semop(sem, &sem_inc, 1);
}

/*Display the first 'dim' characters of the string arr[] */
void print_file_contents(char arr[], int dim)
{
	printf("\nCurrent resources\n\n");
	for(int i = 0; i < dim; i++)
	{
		printf("%c", arr[i] );
	}

}

/*Check that the given array arr[] contains num*/
int contains_val(int arr[], int num, int dim)
{
	int count = 0;
	for(int i = 0;i < dim; i++)
	{	
		if(arr[i] == num)
			count++;
	}

	return count;
}

/*Parent process - it acts as the provider of resources. Continuously asking
for user input and adding the resources to 'file_contents' (mmap'ed file)*/
void parent_process(char *file_contents, int length, pid_t pid)
{
	char response, nl;
	int arr_count = 0;
	int resource_arr[9];
	/*Get resource types*/
	for(int i = 0; i < length - 2; i = i + 4)
	{
		resource_arr[arr_count] = file_contents[i] - '0';
		arr_count++;
	}
		
	do
	{
		/*Find what the user wants to add*/
		printf("Do you want to add more of a resource? Y-yes, E-exit\n");
		response = getchar();
		nl = getchar();
		if(response == 'Y' || nl == 'Y')
		{
		
			int resource;
			while(1)
		    {
		    	printf("What resource would you like to add?\n");
		        if (scanf("%d", &resource)==1 && resource <= 9 && resource >= 0)
		        {
		        	if(contains_val(resource_arr, resource, arr_count))
		            	break;
		            else{
		            	printf("error: value given is not one of the options\n");
		            	getchar();
		            }
		        }

		        else
		        {
		            printf("error: enter an integer in range\n");
		            getchar();
		        }
		    }

		    int add;
			printf("How many of resource %d you like to add?\n", resource);
		    while(1)
		    {
		        if (scanf("%d", &add)==1 && add >= 0)
		            break;
		        else
		        {
		            printf("Error! Enter a new value\n");
		            getchar();
		        }
		    }

		    /*Critical section, add the resources. If user asks to put more than allowed
		    then just fill the resource up*/
		    
		    sem_wait();
		    int num_resources, index;
		    for(int i = 0; i < length; i = i + 4)
		    {
		    	if(file_contents[i] - '0' == resource)
		    	{
		    		index = i + 2;
		    		num_resources = file_contents[index] - '0';
		    		if(num_resources == 9)
		    		{
		    			printf("\nResources already full!\n");

		    		}
		    		if(num_resources + add  <= 9)
		    		{
		    			file_contents[index] = num_resources + add + '0';
		    		}
		    		else
		    		{
		    			printf("\nFilling resources up!\n");
		    			file_contents[index] = 9 + '0';
		    		}
		    	}
		    }
		    sem_signal();
		    /*sync file*/
		    msync((void*)file_contents, length, MS_SYNC);

		}
		else if(response == 'E' || nl == 'E')
		{
			printf("Exiting....\n");
			kill(pid, SIGKILL);
		}

	}while(response != 'E' && nl != 'E');

}//end of parent_process

/*The job of the child process is to report what has happened with the
 mmap'ed file over 10 second intervals*/
void child_process(char *file_contents, int length)
{
	do
	{
		printf("\n------Memory reporter------\n");
		/*Gather  information to display*/
		sem_wait();
		int pg = getpagesize();
		char *vec = (char *)malloc(((pg + length - 1)/(pg))*(sizeof(char)));
		mincore(file_contents, length, vec);
		print_file_contents(file_contents, length);
		sem_signal();
		/*print and free*/
		printf("Current page size = %d\n", pg);
		printf("Page in main mem (Y-1, N-0)? %d\n", *vec & 1);
		printf("-------End of report-------\n");
		free(vec);
		sleep(10);

	}while(1);

}//end of child_process