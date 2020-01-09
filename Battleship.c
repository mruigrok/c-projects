#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 10
#define COLS 10

void shipPlacement(int (*mat) [COLS], int shipNumber);
int guessShip(int (*mat)[COLS], int (*mat2)[COLS]);
void printEmptyBoard();
int correctValue(int x);

//ruigrokm 400147452
//SFWRENG 2MP3 Battleship Assignment

void main(){

	printf("**************************\n* Welcome to Battleship! *\n**************************\n");
	sleep(3);
	printf("We will be playing on 10x10 boards\nEach player will receive 3 ships to place\n\n");
	printEmptyBoard();
	int playerBoard1[ROWS][COLS]={0};
	int playerBoard2[ROWS][COLS]={0};
	int gameBoard1[ROWS][COLS]={0};
	int gameBoard2[ROWS][COLS]={0};

	printf("Give Player 1 the computer to place their ships\n");
	sleep(3);

	shipPlacement(playerBoard1,4);
	shipPlacement(playerBoard1,3);
	shipPlacement(playerBoard1,2);

	printf("All ships placed! Give Player 2 the computer to place their ships\n");
	printf("Screen will be cleared to ensure no cheating\n");

	sleep(6);
	system("clear");

	printf("**************************\n* Welcome to Battleship! *\n**************************\n");
	printf("We will be playing on 10x10 boards\nEach player will receive 3 ships to place\n\n");
	printEmptyBoard();
	shipPlacement(playerBoard2,4);
	shipPlacement(playerBoard2,3);
	shipPlacement(playerBoard2,2);
	printf("Ship are place!\nScreen will be cleared to ensure no cheating\n");

	sleep(6);
	system("clear");



	printf("Let's play!\nA hit ship will be represented by an 'H', while a miss will be an 'X'\n");
	sleep(3);
	printf("Player 1 guesses first\n");
	int player1Hits=0;
	int player2Hits=0;

	while (player1Hits<12 && player2Hits<12){
		printf("Player 1's guesses\n");
		player1Hits+=guessShip(playerBoard2, gameBoard2);

		if (player1Hits==12){
			break;
		}
		printf("Pass to player 2\n");

		sleep(2);
		system("clear");

		printf("Player 2's guesses\n");
		player2Hits+=guessShip(playerBoard1, gameBoard1);

		if (player2Hits==12){
			break;
		}
		printf("Pass to player 1\n");

		sleep(2);
		system("clear");

	}

	sleep(3);

	if (player1Hits>player2Hits){
		printf("\nCongratulations Player 1 you sunk all the ships and are the winner!\n\n");
	}
	else if(player2Hits>player1Hits){
		printf("\nCongratulations Player 2 you sunk all the ships and are the winner!\n\n");
	}

	sleep(2);
	printf("Thanks for playing!\n");

} // end of main function

void printEmptyBoard(){

	printf("   ");

	for (int i=0;i<10;i++){
		printf("%d  ", i);
	}
	printf("\n");
	for (int i=0;i<10;i++){
		printf("%d  ", i );
		for (int j=0;j<10;j++){
			printf("0  ");
		}
		printf("\n");
	}
	printf("\n");
}

void shipPlacement(int (*mat)[COLS], int shipNumber){
	int row, col, orient;
	int up=0,down=0,left=0,right=0;
	for (int i=0;i==0;){

		printf("Place %d unit ship\n", shipNumber+1 );
		printf("Please enter the row you want to place ship\n");
		row=correctValue(9);
		printf("Please enter the column you want to place ship\n");
		col=correctValue(9);

		if (mat[row][col]!=0){
			printf("Ship already there! Enter a new location\n\n");
		}

		else{
		 //The next 4 if statements check to see where the ship can be placed
			if (row-shipNumber>=0 && mat[row][col]==0){
				int a=0;
				for (int i=0;i<=shipNumber;i++){
					if (mat[row-i][col]==0){
						a+=0;
					}
					else{
						a+=1;
					}
				}
				if (0==a){
					printf("1:Up ");
					up=1;
				}
			}

			if (row+shipNumber<=9 && mat [row][col]==0){
				int a=0;
				for (int i=0;i<=shipNumber;i++){
					if (mat[row+i][col]==0){
						a+=0;
					}
					else{
						a+=1;
					}
				}
				if (0==a){
					printf("2:Down ");
					down=1;
				}
			}

			if (col-shipNumber>=0 && mat[row][col]==0){
				int a=0;
				for (int i=0;i<=shipNumber;i++){
					if (mat[row][col-i]==0){
						a+=0;
					}
					else{
						a+=1;
					}
				}
				if (0==a){
					printf("3:Left ");
					left=1;
				}
			}

			if (col+shipNumber<=9){
				int a=0;
				for (int i=0;i<=shipNumber;i++){
					if (mat[row][col+i]==0){
						a+=0;
					}
					else{
						a+=1;
					}
				}
				if (0==a){
					printf("4:Right");
					right=1;
				}
			}

			if (up>0 || down>0 || left>0 || right>0)
				i++;
			else
				printf("No orientation to place ship! Enter new coordinates\n");

		}
	}

	// Ensuring you can place ship in certain direction..then placing it on board
	for (int correct=0;correct<1;){
		printf("\nWhat orientation from above would you like ?\n");
		orient=correctValue(4);
		if (orient==1 && up==1){
			for (int i=0;i<=shipNumber;i++){
				mat[row-i][col]=1;
			}
			correct++;
		}

		else if (orient==2 && down==1){
			for (int i=0;i<=shipNumber;i++){
				mat[row+i][col]=1;
			}
			correct++;
		}

		else if (orient==3 && left==1){
			for (int i=0;i<=shipNumber;i++){
				mat[row][col-i]=1;
			}
			correct++;
		}

		else if (orient==4 && right==1){
			for (int i=0;i<=shipNumber;i++){
				mat[row][col+i]=1;
			}
			correct++;
		}
		else{
			printf("Cannot place ship there!!\n");
		}
	}

	//printing the updated board out to see
	printf("   ");

	for (int i=0;i<10;i++){
		printf("%d  ", i);
	}
	printf("\n");
	for (int i=0;i<10;i++){
		printf("%d  ", i );
		for (int j=0;j<10;j++){
			printf("%d  ", mat[i][j]);
		}
		printf("\n");
	}
	printf("\n");

return;

}

int guessShip(int (*mat)[COLS], int (*mat2)[COLS]){
	int row, col;

	printf("   ");

	for (int i=0;i<10;i++){
		printf("%d  ", i);
	}

	printf("\n");

	for (int i=0;i<10;i++){
		printf("%d  ", i );
		for (int j=0;j<10;j++){
			if (mat2[i][j]==2){
				printf("X  ");     //A miss will be an X
			}
			else if (mat2[i][j]==1){
				printf("H  ");		// A hit is an H
			}
			else{
				printf("0  ");
			}
		}
		printf("\n");
	}

	for (int i=0;i==0;){

		printf("\nEnter the row of your guess\n");  //getting coordinates of guesses
		row = correctValue(9);
		printf("Enter the column of your guess\n");
		col= correctValue(9);

		if (mat[row][col]==1 && mat2[row][col]==0){
			printf("Hit!!\n");
			mat2[row][col]=1;
			i++;
			return 1;
		}

		else if (mat[row][col]==0 && mat2[row][col]==0){
			printf("Miss\n");
			mat2[row][col]=2;
			i++;
			return 0;

		}

		else{
			printf("Already guessed there!! Guess again\n");

		}

	}
}


// Use this function to make sure an integer in the correct range is entered
int correctValue(int x){
	int value=0;
	for (int i=0;i==0;){
		if ((scanf("%d", &value)==1 && value>=0 && value<=x)){
			i++;
		}

		else{
			printf("Error! Enter an integer in range\n");
			getchar();

		}
	}
	return value;
}
