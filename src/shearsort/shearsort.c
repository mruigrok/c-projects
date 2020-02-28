#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include <limits.h>
#include <math.h>

/*
SFRW ENG 3SH3 Lab 3
Threads
Matthew Ruigrok
400147452
*/

/*4x4 matrix for 16 entries, define the row/column vector sizes as VEC_SIZE,
INC is for sorting in increasing order, DEC in decreasing order*/

#define INC 0
#define DEC 1


/*Defining the type and size of the items/array we are going to sort*/
int VEC_SIZE;
int **A;

/*Function declaration*/
void write_to_file(void);
void read_in_array(char *);
void print_matrix(int **);
void col_bubble_sort(int);
void row_bubble_sort(int,int);
int correct_integer(void);
void itr_start(void);
void itr_end(void);
void *shearsort(void *);

/*Global Variables*/
int thr_st = 0;
int thr_end = 0;

/*Threads, Mutex and Condition variables*/
pthread_mutex_t thr_st_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t thr_st_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t thr_end_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t thr_end_cond = PTHREAD_COND_INITIALIZER;


/****Function defintions****/


/*Write the VEC_SIZE x VEC_SIZE array input by user to a file*/
void write_to_file(void){
	/*Open file and write in user input*/
	FILE *fp;
	int num;
	fp = fopen("input.txt", "w");
	if(fp == NULL){
		printf("error: did not open file!\n");
		exit(1);
	}
	else{
		/*Take user input and write*/
		printf("How many values are you entering?\n");
		int size = correct_integer();
		int num_row = sqrt(size);
		//int num_row = 4;
		fprintf(fp, "%d\n", num_row);
		for(int i = 0; i < num_row; ++i ){
			for (int j = 0; j < num_row; ++j){
				printf("Enter a value for Row %d Col %d\n", i + 1, j + 1);
				num  = correct_integer();
				fprintf(fp, "%d ", num);
			}
		}
		fclose(fp);
	}
}

/*Reads in the VEC_SIZE x VEC_SIZE array from the corresponding text file*/
void read_in_array(char *path){
	/*Read in the array from input.txt*/
	FILE *fp;
	fp = fopen(path, "r");
	if(fp == NULL){
		printf("error: did not open file!\n");
		exit(1);
	}
	else{
		int num;
		fscanf(fp, "%d\n", &VEC_SIZE);

		A = malloc(VEC_SIZE*sizeof(int*));
		for(int k = 0; k < VEC_SIZE; k++){
			A[k] = malloc(VEC_SIZE*sizeof(int));
		}

		/*Read in the file*/
		for(int i = 0; i < VEC_SIZE; ++i ){
			for (int j = 0; j < VEC_SIZE; ++j){
				fscanf(fp, "%d ", &num);
				A[i][j] = num;
			}
		}
		fclose(fp);
	}
	
}

/*Print the matrix to the screen*/
void print_matrix(int **A){
	for(int i = 0; i < VEC_SIZE; ++i ){
		for (int j = 0; j < VEC_SIZE; ++j){
			printf("%d\t", A[i][j] );
		}
		printf("\n");
	}
	printf("\n");
}

/*Sort col of Matrix A in ascending order*/
void col_bubble_sort(int col){
	if(col < VEC_SIZE){
		for(int i = 0; i < VEC_SIZE - 1; i++){
			for (int j = 0; j < VEC_SIZE - i -1; j++){
				if(A[j][col] > A[j+1][col]){
					int temp = A[j][col];
					A[j][col] = A[j+1][col];
					A[j+1][col] = temp;
				}	
			}
		}
	}
	else{
		printf("Error: Given column out of bounds\n");
		exit(1);
	}
}

/*Sorts row of Matrix A  in ascending order if order = INC or descending order if order = DEC*/
void row_bubble_sort(int row, int order){
	if(row < VEC_SIZE){
		for(int i = 0;i < VEC_SIZE - 1;i++){
	    	for(int j = 0;j < VEC_SIZE -i -1;j++){
		    	if(order == INC){
		    		if(A[row][j] > A[row][j+1]){
			        	int temp = A[row][j+1];
			        	A[row][j+1] = A[row][j];
			        	A[row][j] = temp;
		      		}
		      	}
		    	else{
		    		if(A[row][j] < A[row][j+1]){
			        	int temp = A[row][j+1];
			        	A[row][j+1] = A[row][j];
			        	A[row][j] = temp;
		      		}
		    	}
	    	}	
		}
	}	
	else{
		printf("Error: Given row out of bounds\n");
		exit(1);
	}
}

/*Ensure that a correct integer is entered*/
int correct_integer(void){
	int value;
    while(1){
        if (scanf("%d", &value)==1 && value < INT_MAX && value > INT_MIN)
            break;

        else{
            printf("Error! Enter an integer in range\n");
            getchar();
        }
    }
    return value;
}

/*Wait at the beginning of each thread for all others*/
void itr_start(void){
	/*Increment the count*/
	pthread_mutex_lock(&thr_st_mutex);
	thr_st++;
	
	/*Wait until all of the threads, then release*/
	if(thr_st != VEC_SIZE){
		pthread_cond_wait(&thr_st_cond, &thr_st_mutex);
	}
	
	pthread_cond_signal(&thr_st_cond);
	thr_st--;
	pthread_mutex_unlock(&thr_st_mutex);
}

/*Wait at the end of each thread for all others*/
void itr_end(void){
	/*Increment the count*/
	pthread_mutex_lock(&thr_end_mutex);
	thr_end++;
	
	/*Wait for all of the threads, then release*/
	if(thr_end != VEC_SIZE){
		pthread_cond_wait(&thr_end_cond, &thr_end_mutex);
	}

	pthread_cond_signal(&thr_end_cond);
	thr_end--;
	pthread_mutex_unlock(&thr_end_mutex);

}

/*Sorting algorithm, arg is the thread id that corresponds 
with the row/column to work on in the matrix*/

void *shearsort(void *arg){
	/*Init a thread ID for each thread, work on that column or row*/
	int thread_id;
    thread_id = (int)arg;

    /*To keep track of the number of iterations*/
	int itr;
	int max_itr = ceil(log2(VEC_SIZE*VEC_SIZE)) + 1;

	for(itr = 0; itr < max_itr; itr++){

		/*Make sure all threads are here */
		
		itr_start();
	
		if((itr + 1) % 2 == 0){
			/*Even phase, operate on column*/
			printf("Working on Col: %d\n", thread_id + 1);
			col_bubble_sort(thread_id);
		}
		else{
			if(thread_id % 2 == 0){
				/*Even column, sort in ascending order*/
				printf("Working on Row: %d\n", thread_id + 1);
				row_bubble_sort(thread_id, INC);
			}
			else{
				/*Odd column, sort in descending order*/
				printf("Working on Row: %d\n", thread_id + 1);
				row_bubble_sort(thread_id, DEC);
			}
		}

		/*Wait until all threads have finished their iteration*/

		itr_end();
		
		/*Print matrix once for each iteration*/
		if(thread_id == 0){
			printf("\nMatrix after iteration %d\n", itr + 1);
			print_matrix(A);
		}
				
	}

	pthread_exit(NULL);

}//end of shearsort

int main(void)
{	
	int rc, t;

  	/*Initialize the matrix*/
  	
  	read_in_array("input.txt");

  	printf("\n\n");

  	/*Create threads with ID*/

  	pthread_t tid[VEC_SIZE];
  	for(t = 0; t < VEC_SIZE; t++){
  		printf("Creating thread %d.....\n\n",  t + 1);
  		rc = pthread_create(&tid[t], NULL, shearsort, (void*)t);
  		if(rc){
  			printf("error: did not create thread");
  			exit(1);
  		}
  	}

  	printf("\n\n");

  	/*Join the threads when finished*/

  	for(t = 0; t < VEC_SIZE; t++){
  		pthread_join(tid[t], NULL);
  	}
  	
  	printf("**Matrix after Shearsort**\n");
  	
  	print_matrix(A);

  	/*Clean up*/

  	pthread_mutex_destroy(&thr_st_mutex);
  	pthread_mutex_destroy(&thr_end_mutex);
  	pthread_cond_destroy(&thr_st_cond);
  	pthread_cond_destroy(&thr_end_cond);
  	pthread_exit(NULL);

}//end of main()