//Easy - 100904082052680300864200910010049806498300701607010093086035209509002130030497008
//Mid - 100070030830600000002900608600004907090000050307500004203009100000002043040080009
//Hard - 000700000100000000000430200000000006000509000000000418000081000002000050040000300
//Unsolvable - 200900000000000060000001000502600407000004100000098023000003080005010000007000000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//Each possibilityArray has an array of 9 "possibilities"
//-Number is the value of the possibility (each nummber from 1 to 9)
//-Valid is a boolean that tells if the number is actually usable.
struct possibility{
	int number;
	int valid;
};

//Each cell has a "possibilityArray"
//-Available is the total numbers that are available for its cell. 
//-Array is an array of the struct below. It contains the numbers 1 to 9.
struct possibilityArray{
	struct possibility array[9];
	int available;
};

//Each sudoku has 81 "cells",
//-Number is the current value of the cell (from 1 to 9) 
//-Attempts is the number of guesses that have been made; it gets reset when moving back to a previous cell
//-Head is a struct (see below)
//maybe add row and col ? so can pass the cell instead? mmhh
struct cell{
	int number;
	int attempts;
	struct possibilityArray head;
};

//When making a guess for a cell, a "guess" struct gets created.
//Number_guessed is the number that has been guessed for the cell
//Row is the row of the cell that has been guessed
//Column is the column of the cell that has been guessed
struct guess{
	int row;
	int column;
	int number_guessed;
};

//Debug function to print the numbers that are possible for a given cell
void print_array_of_possibility(struct cell matrix[9][9], int row, int column){
	for(int i=0; i<9; i++){
		if(matrix[row][column].head.array[i].valid) printf("%d, ", matrix[row][column].head.array[i].number);
	}
	printf("; Available: %d\n", matrix[row][column].head.available);
}

//Function to print the current status of the sudoku matrix
void print_matrix(struct cell matrix[9][9]){
	printf("\e[1;1H\e[2J"); //Clears the console much more quickly than system(clear)
	for (int i=1; i<10; i++){
		for (int j=1; j<10; j++){
			printf("%d ", matrix[i-1][j-1].number);
			if(j%3==0) printf(" ");
		}
		printf("\n");
		if(i%3==0) printf("\n");
	}
}

//Function that takes a string and builds the matrix and the relative cells and info for it.
//TODO: Sanitize the input and make sure it only works with a 81 numbers long input.
//TODO: USE QUOTIENT AND REMAINDER U FUCKIND DUMB IDIOT
int take_input(struct cell matrix[9][9]){
	int solved = 0;

	int a[81];
	for(int i=0;i<81;i++){
		scanf("%1d", &a[i]);
	}
	int n=0;
	for(int i=0;i<9;i++){
		for (int j=0; j<9; j++){
			matrix[i][j].number = a[n];
			matrix[i][j].attempts = 0;
			for(int w=0; w<9; w++){
					matrix[i][j].head.array[w].number = w+1;
					matrix[i][j].head.array[w].valid = 1;
			}
			if(a[n]>0){
				solved++;
				matrix[i][j].head.available = 0;
			}
			else{
				matrix[i][j].head.available = 9;
			}
			n++;
		}
	}
	return solved;
}

//Function that checks all the numbers in the same row, column and square of the given cell and
//it removes the numbers that can't be in the given cell from the array of possible numbers.
//TODO: Improve search in same square
int find_cell_possibilities(struct cell matrix[9][9], int row, int column){
	int n;
	//Check all the rows in the given column and updates the array of possibilities and the valid numbers
	for(int i=0; i<9; i++){
		n = matrix[i][column].number;
		if(n!=0 && matrix[row][column].head.array[n-1].valid){		
			matrix[row][column].head.array[n-1].valid = 0;
			//matrix[row][column].head.available -= 1;
		}
	}

	//check all the columns in the given row and updates the array of possibilities and the valid numbers
	for(int i=0; i<9; i++){
		n = matrix[row][i].number;
		if(n!=0 && matrix[row][column].head.array[n-1].valid){
			matrix[row][column].head.array[n-1].valid = 0;
			//matrix[row][column].head.available -= 1;
		}
	}


	//MAYBE use switch 
	//TopLeft cell; Checks the square and updates the array of possibilities and the valid numbers
	if(row%3 == 0 && column%3 == 0){
		for (int i = row; i < row+3; i++){
			for(int j = column; j < column+3; j++){
				n = matrix[i][j].number;
				if(n != 0 && matrix[row][column].head.array[n-1].valid){
					matrix[row][column].head.array[n-1].valid = 0;
					//matrix[row][column].head.available -= 1;
				}
			}
		}
	}


	//TopMid cell; Checks the square and updates the array of possibilities and the valid numbers
	else if(row%3 == 0 && column%3 == 1){
		for (int i = row; i < row+3; i++){
			for(int j = column-1; j < column+2; j++){
				n = matrix[i][j].number;
				if(n != 0 && matrix[row][column].head.array[n-1].valid){
					matrix[row][column].head.array[n-1].valid = 0;
					//matrix[row][column].head.available -= 1;
				}
			}
		}
	}

	//TopRight cell; Checks the square and updates the array of possibilities and the valid numbers
	else if(row%3 == 0 && column%3 == 2){
		for (int i = row; i < row+3; i++){
			for(int j = column-2; j < column+1; j++){
				n = matrix[i][j].number;
				if(n != 0 && matrix[row][column].head.array[n-1].valid){
					matrix[row][column].head.array[n-1].valid = 0;
					//matrix[row][column].head.available -= 1;
				}
			}
		}
	}

	//MidLeft cell; Checks the square and updates the array of possibilities and the valid numbers
	else if(row%3 == 1 && column%3 == 0){
		for (int i = row-1; i < row+2; i++){
			for(int j = column; j < column+3; j++){
				n = matrix[i][j].number;
				if(n != 0 && matrix[row][column].head.array[n-1].valid){
					matrix[row][column].head.array[n-1].valid = 0;
					//matrix[row][column].head.available -= 1;
				}
			}
		}
	}

	//Mid cell; Checks the square and updates the array of possibilities and the valid numbers
	else if(row%3 == 1 && column%3 == 1){
		for (int i = row-1; i < row+2; i++){
			for(int j = column-1; j < column+2; j++){
				n = matrix[i][j].number;
				if(n != 0 && matrix[row][column].head.array[n-1].valid){
					matrix[row][column].head.array[n-1].valid = 0;
					//matrix[row][column].head.available -= 1;
				}
			}
		}
	}

	//MidRight cell; Checks the square and updates the array of possibilities and the valid numbers
	else if(row%3 == 1 && column%3 == 2){
		for (int i = row-1; i < row+2; i++){
			for(int j = column-2; j < column+1; j++){
				n = matrix[i][j].number;
				if(n != 0 && matrix[row][column].head.array[n-1].valid){
					matrix[row][column].head.array[n-1].valid = 0;
					//matrix[row][column].head.available -= 1;
				}
			}
		}
	}

	//BotLeft cell; Checks the square and updates the array of possibilities and the valid numbers
	else if(row%3 == 2 && column%3 == 0){
		for (int i = row-2; i < row+1; i++){
			for(int j = column; j < column+3; j++){
				n = matrix[i][j].number;
				if(n != 0 && matrix[row][column].head.array[n-1].valid){
					matrix[row][column].head.array[n-1].valid = 0;
					//matrix[row][column].head.available -= 1;
				}
			}
		}
	}

	//BotMid cell; Checks the square and updates the array of possibilities and the valid numbers
	else if(row%3 == 2 && column%3 == 1){
		for (int i = row-2; i < row+1; i++){
			for(int j = column-1; j < column+2; j++){
				n = matrix[i][j].number;
				if(n != 0 && matrix[row][column].head.array[n-1].valid){
					matrix[row][column].head.array[n-1].valid = 0;
					//matrix[row][column].head.available -= 1;
				}
			}
		}
	}

	//BotRight cell; Checks the square and updates the array of possibilities and the valid numbers
	else if(row%3 == 2 && column%3 == 2){
		for (int i = row-2; i < row+1; i++){
			for(int j = column-2; j < column+1; j++){
				n = matrix[i][j].number;
				if(n != 0 && matrix[row][column].head.array[n-1].valid){
					matrix[row][column].head.array[n-1].valid = 0;
					//matrix[row][column].head.available -= 1;
				}
			}
		}
	}
	
	//Update the "available" numbers and return it
	int x = 0;
	for (int i=0; i<9; i++){
		if (matrix[row][column].head.array[i].valid) x++;
	}
	matrix[row][column].head.available = x;
	return x;
}

//Function that resets the array of possible numbers of a cell when going back to a previous cell
void reset_array_of_possibilities(struct cell matrix[9][9], int row, int column){
	for(int i=0; i<9; i++){
		matrix[row][column].head.array[i].valid = 1;
		matrix[row][column].head.available = 9;
	}
}

//Function that actually solves the sudoku.
//First it tries solving the "naked singles" until there are no more
//and than moves to the backtracking algorithm
//TODO: implement solving of more complex cases before going for the backtracking alg
void solve(struct cell matrix[9][9], int solved){

	int starting = solved;

	//Until the sudoku is solved
	while(solved < 81){
		
		//Scan all cell to find one with only one possibility available (aka a "naked single")
		for (int i=0; i<9; i++){
			for (int j=0; j<9; j++){
				if (matrix[i][j].head.available == 1){

					//If only one number is available, find it
					int n = 0;
					while(!matrix[i][j].head.array[n].valid) n++;

					//Set the cell to that number, make it non valid and decrement the number of available options
					matrix[i][j].head.array[n].valid = 0;
					matrix[i][j].head.available -= 1;
					matrix[i][j].number = n+1;

					//Increment the solved cells
					solved ++;
					print_matrix(matrix);

				}
			}
		}
		

		//This condition is true only when there are no more "naked singles"
		//It breaks out of the while loop and moves to the backtracking alg
		if(starting == solved){
			//printf("NEED TO CHANGE TO BACKTRACKING ALGORITHM\n");
			break;
		}

		//Calculate possibilities of each cell again before the next iteration of the loop
		for (int i=0; i<9; i++){
			for (int j=0; j<9; j++){ 
				if(matrix[i][j].number == 0) 
					if(!find_cell_possibilities(matrix, i, j)){
						printf("Unsolvable\n");
						return;
					}
			}
		}

		//update the variable so that the exit conditoin is not always true at the second iteration
		starting = solved;
	}


	//BACKTRACKING ALGORITHM
	//Starting from the first unsolved cell, choose an available number and move to the next cell
	//If the next cell has at least one available numbers repeat
	//If the next cell has no available numbers, move back to the previous cell that was guessed
	//and choose the next available number


	//Put all the cells that have been guessed in an array so we can backtrack
	//We use a new struct bc its easier maybe i'm actually not sure
	//r and c are the row and column of the cell that is currently in the array at position [array_head]
	int r, c, array_head = 0;
	int size = 81 - solved; //Number of cells to solve
	struct guess array_of_guesses[size]; //Array of guessed cell

	while(solved < 81){

		//If we have tried all available numbers for a cell, we reset its array of possibilities
		//and move back to the previous cell guessed
		if(matrix[r][c].attempts >= 9){
			if(array_head == 0){
				//The first cell that was guessed has tried all numbers, so the sudoku is unsolvable
				printf("Unsolvable");
				break;
			}
			//printf("[%d][%d] has tried all numbers, going back\n", r, c);
			reset_array_of_possibilities(matrix, r, c);
			matrix[r][c].attempts = 0;
			array_head--;
			solved--;
			matrix[array_of_guesses[array_head].row][array_of_guesses[array_head].column].number = 0;
			//Need to update r & c because the next iteration could also enter this part of the code
			r = array_of_guesses[array_head].row;
			c = array_of_guesses[array_head].column;
			continue; //Force the next iteration of the while loop
		}

		//Scan each cell
		for (int i=0; i<9; i++){
			for (int j=0; j<9; j++){

				if(matrix[i][j].number == 0){	//The cell is empty and has to be guessed
					//Update the possibile numbers of the cell
					//If there are no available numbers, reset the current cell
					//And move back to the previous cell guessed
					if(!find_cell_possibilities(matrix, i, j)){
						matrix[i][j].attempts = 0;
						reset_array_of_possibilities(matrix, i, j);
						array_head--;
						solved--;
						int prev_row = array_of_guesses[array_head].row;
						int prev_col = array_of_guesses[array_head].column;
						r=prev_row, c=prev_col;
						matrix[prev_row][prev_col].number = 0;
						i=j=9; //Needed to exit the nested fors
						print_matrix(matrix);
						break;
					}
					
					//If there's at least one available number,
					//find the first number that is valid and that hasn't been tried yet. 
					else{
						int attempts = matrix[i][j].attempts;
						//Find the next available number
						while(!matrix[i][j].head.array[attempts].valid && attempts<=9) attempts++;
						int number_guess = matrix[i][j].head.array[attempts].number;
						matrix[i][j].attempts = attempts;
						
						//If the available number(s) have already been tried
						if(attempts >= 9){
							i=j=9;
							break; //Exit for loops
						}

						//Put the guess and its info in the array
						array_of_guesses[array_head].number_guessed = number_guess;
						array_of_guesses[array_head].row = i;
						array_of_guesses[array_head].column = j;
						array_head++; 

						//Put the guess in the sudoku matrix and update the necessary info
						matrix[i][j].number = number_guess;
						matrix[i][j].attempts++;
						solved++;
						print_matrix(matrix);
					}
				}
			}
		}
	}
}

int main(){
	struct cell matrix[9][9];

	int solved = take_input(matrix);
	printf("Solved %d/81 cells\n", solved);

	//Find the possible numbers for every cell
	for (int i=0; i<9; i++){
		for (int j=0; j<9; j++){ 
			if(matrix[i][j].number == 0) find_cell_possibilities(matrix, i, j);
		}
	}

	clock_t begin = clock();
	solve(matrix, solved);
	clock_t end = clock();

	print_matrix(matrix);
	double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("Execution time: %lf s\n", time_spent);
}