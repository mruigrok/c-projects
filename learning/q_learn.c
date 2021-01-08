#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 
    Play the count to 20 game by each player adding 1 or 2, winner is the one who says 20 
    Trying to arrive at the optimal policy using Q Learning, can change the number of iterations,
    reward function etc.
*/

int  score;   /* Globals */
char in[20];

#define NUM_ITR 150		// Number of iterations for Q Learning

/* ---------------------------- Global variables for QLearn -------------------------*/

double q_val[21][2];	// Store the Q-values
int policy[21];			// Store the policy (what move to make) at each state
double eps; 			// For adding randomness as we make model
double alpha; 			// The learning rate
double gam; 			// The discount constant
int num_itr;			// Number of iterations

/*------------------------------ Functions for Q Learning ---------------------------*/

void init_world(int itr)
{
	for(int i = 0; i <= 20 ; ++i)
	{
		q_val[i][0] = 0.0;
		q_val[i][1] = 0.0;
		policy[i] = 0;
	}
	srand(time(NULL)); 						// Initialize the random seed
	eps = 1;								// Initialize epsilon value
	alpha = 0.2; 							// Default alpha and gamma values
	gam = 0.9;								// Discount rate
	num_itr = itr;							// Number of iterations
}

int choose_action(int count)
{
	int action;
	double e = rand() % RAND_MAX;	    // Choose more random actions as the number of games played increases
	if(eps < e)
		action =  rand() % 2 + 1;   // Random move (between 1 and 2)
	else
	{
		double m_reward = -10000;
		for(int i = 1; i <= 2; ++i)
		{
			if(q_val[count][i] > m_reward)
			{
				m_reward = q_val[count][i];
				action = i;
			}
		}
	}
	return action;	
}

void make_policy(void)
{
	int action;
	double m_reward;
	for(int i = 0; i <= 20; ++i)
	{
		if(q_val[i][0] > q_val[i][1])
		{
			m_reward = q_val[i][0];
			action = 1;
		}
		else
		{
			m_reward = q_val[i][1];
			action = 2;
		}
		policy[i] = action;
	}
}

void print_policy()
{
	FILE *fptr;
	fptr = fopen("policy.txt","w");
	fprintf(fptr, "Count\tPolicy\tOptimal\n");
	for(int i = 0; i < 20; ++i)
	{
		printf("%d: %d\n", i, policy[i]);
		fprintf(fptr, "%d\t%d\t", i, policy[i]);
		if(i % 3 == 1)
			fprintf(fptr, "1\n");
		else if(i % 3 == 0)
			fprintf(fptr, "2\n");
		else
			fprintf(fptr, "rand\n");
	}
	fclose(fptr);
}

int get_reward(int count)
{
	if(count == 20)
		return 2;
	else if(count == 19 || count == 18)
		return -1;
	else if(count % 3 == 2)
		return 1;
	else
		return 0;
}

void iterate(void)
{
	int count_hold, action, count = 0;
	double reward, max_q;
	while(count <= 20)
	{
		/* Get the reward and then find the max Q value for the current count */
		count_hold = count;
		action = choose_action(count);
		count += action;
		if(count <= 20)
		{
			reward = get_reward(count);
			if(q_val[count][0] > q_val[count][1])
				max_q = q_val[count][0];
			else
				max_q = q_val[count][1];
			q_val[count_hold][action - 1] += alpha * (reward + gam * (max_q - q_val[count_hold][action - 1]));
		}
	}
	/* Decay */
	eps = eps * 0.95;					
	if(eps < 0.01)	
		eps = 0.01;
	printf("NEXT EP\n");
}

void run(void)
{
	/* Run the Q learning algorithm with NUM_ITR iterations */
	init_world(NUM_ITR); 					
	for(int i = 0; i < num_itr ; ++i)
		iterate();
	make_policy();
	print_policy();
}

void play_vs_computer(int random)
{
	/* Test the policy computed by Q learning by playing the given computer policy */
	int count = 0; int comp_move;
	while(count < 20)
	{
		printf("User move: %d", policy[count]);
		count += policy[count];
		printf("\tcount: %d\n", count);
		if(count == 20)
		{
			printf("\nUser wins!!\n\n");
			break;
		}
		if(!random)										// Use given computer policy
		{
			if(count % 3 == 1) comp_move = 1;			// Add a 1
			else if(count % 3 == 0) comp_move = 2;		// Add a 2
			else comp_move = rand() % 2 + 1; 			// Add random
		}
		else
			comp_move = rand() % 2 + 1;					// Completely random move
		printf("Comp move: %d", comp_move);
		count += comp_move;
		printf("\tcount: %d\n", count);
	}
}
/*------------------------------ End of Functions for Q Learning ---------------------------*/

int user(void)
{
 int i=0;
 while(i!=1 && i!=2){
    printf("We are at %-2d, add 1 or 2 ? ",score);
    fgets(in,10,stdin);
    i=atoi(in);
  };
 return(i);
}

int computer(void)
{
 /* Get the computer's move from the computed policy */
 int i = policy[score];
 printf("We are at %-2d Computer adds %d \n",score,i);
 return(i);
}

int computer2(void)
{
 if (score% 3==1) return(1);
 if (score %3==0)return(2);
 else return(rand() %2+1);
}

int main(void)
{
  /* Run the q - value iterations first to generate the policy, then play 10 games to verify */
  run();
  for(int i = 0; i < 10; ++i)
	play_vs_computer(0);

  /* Let user play the game */
  srand(time(NULL)); 
  int i;
  printf(" Who says first 20 \n \n");
  score=0;
  i=0;
  while(i!=1 && i!=2){
    printf("Who goes first: you=1 computer=2 ? ");
    fgets(in,10,stdin);
    i=atoi(in);
  };
  if(i==2)score=computer();
  while(score <=20){
    score=score+user();
    if(score>=20) {printf(" YOU WIN !!\n ");break;};
    score=score+computer();
    if(score>=20) {printf(" I WIN !! \n ");break;};
 };
return(0);
};
