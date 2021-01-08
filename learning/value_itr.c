#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Globals */
int  score;  
char in[20];
int policy[20];

double reward(int score, int depth, int is_user)
{
    double max_reward, tmp1, tmp2;
    double gamma = pow(0.9, depth);
    if(is_user == 0 && score >= 20) return 10;
    else if(is_user == 1 && score >= 20) return -10;
    if(is_user == 1)
    {
        tmp1 = reward(score + 1, depth + 1, 0);
        tmp2 = reward(score + 2, depth + 1, 0);
        max_reward = (tmp1 > tmp2) ? tmp1 : tmp2;
        return gamma * max_reward;
    }
    else 
    {
        tmp1 = reward(score + 1, depth + 1, 1);
        tmp2 = reward(score + 2, depth + 1, 1);
        max_reward = (tmp1 < tmp2) ? tmp1 : tmp2;
        return gamma * max_reward;
    }
}

double best_move(int count, int print_policy)
{
    double mv1, mv2;
    mv1 = reward(count + 1, 0, 0);
    mv2 = reward(count + 2, 0, 0);
    int best_move = (mv1 < mv2) ? 2 : 1;
    if(print_policy) policy[count] = best_move; // For printing the policy
    else
    {
        printf("At count %d move %d is %f \n", count, 1, mv1);
        printf("At count %d Move %d is %f \n", count, 2, mv2);
    }
    return best_move;
}

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

int computer(int score)
{
 int i = best_move(score, 0);
 printf("We are at %-2d Computer adds %d \n",score,i);
 return(i);
}

void print_policy()
{
	FILE *fptr;
	fptr = fopen("policy.txt","w");
	fprintf(fptr, "Count\tPolicy\tOptimal\n");
	for(int i = 0; i < 20; ++i)
	{
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

int main(void)
{
    int j;
    for(int i = 0; i < 20; ++i)
        j = best_move(i, 1);
    print_policy(); 

  int i;
  printf(" Who says first 20 \n \n");
  score=0;
  i=0;
  while(i!=1 && i!=2){
    printf("Who goes first: you=1 computer=2 ? ");
    fgets(in,10,stdin);
    i=atoi(in);
  };
  if(i==2)score=computer(score);
  while(score <=20){
    score=score+user();
    if(score>=20) {printf(" YOU WIN !!\n ");break;};
    score=score+computer(score);
    if(score>=20) {printf(" I WIN !! \n ");break;};
 };
return(0);
};