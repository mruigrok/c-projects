#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/*---------Global Variables-----------*/

/*Semaphore initialization and incrementing/decrementing*/

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

/*--------Function declaration---------*/

void sem_wait();
void sem_signal();
void print_file_contents(char[], int);
int contains_val(int[], int, int);
void change_resource(char[], int);


/*-----------Main program--------------*/

int main(int argc, char *argv[])
{
	/*Initialize semaphore or locate the old one*/

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

	/*Open up the file, read it's contents an then mmap it*/

	char *filename = argv[1];
	int fd = open(filename, O_RDWR, S_IRUSR | S_IWUSR);
	struct stat sb;

	if(fstat(fd, &sb) == -1)
	{
		perror("couldn't get the file size");
	}

	char *file_contents = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	/*Keep prompting user for allocation*/

	while(1)
	{
		change_resource(file_contents, sb.st_size);
		msync((void*)file_contents, sb.st_size, MS_SYNC);
	}

}//end of main()


/*------Function Definition------*/

/*Semaphore operations*/
void sem_wait()
{
	semop(sem, &sem_dec, 1);
}

void sem_signal()
{
	semop(sem, &sem_inc, 1);
}

/*Display the first 'size' characters of the string arr[] */
void print_file_contents(char arr[], int size)
{
	printf("\n");
	for(int i = 0; i < size; i++)
	{
		printf("%c", arr[i] );
	}
}

/*Check to see if num is in integer array arr of length size*/
int contains_val(int arr[], int num, int size)
{
	int count = 0;
	for(int i = 0;i < size; i++)
	{	
		if(arr[i] == num)
			count++;
	}
	return count;
}

void change_resource(char *file_contents, int length){

	/*Add the available resource types*/
	int arr_count = 0;
	int resource_arr[9];
	for(int i = 0; i < length - 2; i = i + 4)
	{
		resource_arr[arr_count] = file_contents[i] - '0';
		arr_count++;
	}

	/*Prompt the user for what resource they want to allocate and ensure it is
	 a resource that is available to be allocated, get the number available,
	 and then allocate the resources*/

	printf("What resource would you like to allocate?\n");

	int resource;
	while(1)
    {
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

    int subtract;
	printf("How many of resource %d you like to allocate?\n", resource);
    while(1)
    {
        if (scanf("%d", &subtract)==1 && subtract >= 0)
            break;
        else
        {
            printf("Error! Enter a new value\n");
            getchar();
        }
    }

    /*Critical section, allocate the resources. If user asks for more than available
    then just take what is there and short the user*/

    sem_wait();
    int num_resources, index;
    for(int i = 0; i < length; i = i + 4)
    {
    	if(file_contents[i] - '0' == resource)
    	{
    		index = i + 2;
    		num_resources = file_contents[index] - '0';
    		if(num_resources == 0)
    		{
    			printf("\nNo resources available to allocate!\n");
    			file_contents[index] = '0';
    		}
    		else if(num_resources - subtract  >= 0)
    		{
    			file_contents[index] = num_resources - subtract + '0';
    		}
    		else
    		{
    			printf("\nTaking the rest of the resources!\n");
    			file_contents[index] = '0';
    		}
    	}
    }
    printf("\nFile after changing\n");
	print_file_contents(file_contents, length);
    sem_signal();

}//end of change_resource

