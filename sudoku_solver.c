//100904082052680300864200910010049806498300701607010093086035209509002130030497008
//100070030830600000002900608600004907090000050307500004203009100000002043040080009
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

struct possibility{
	int number;
	int valid;
};

struct possibilityArray{
	struct possibility array[9];
	int available;
};
//maybe add row and col ? so can pass the cell instead? mmhh
struct cell{
	int number;
	struct possibilityArray head;
	int attempts;
};


struct guess{
	int row;
	int column;
	int number_guessed;
};



//Debug function to print the numbers that are possible for a given cell
void printArrayOfPossibility(struct cell cell, int row, int column){
	printf("POSSIBILITIES FOR [%d][%d]: ", row+1, column+1);
	for(int i=0; i<9; i++){
		if(cell.head.array[i].valid) printf("%d", cell.head.array[i].number);
	}
	printf("; Available: %d\n", cell.head.available);
}


//Function to print the current status of the sudoku
void print_matrix(struct cell matrix[9][9]){
	//system("clear"); only works on unix and is slow rip
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



//Function that checks all the numbers in the same row and column of the given cell and
//it removes the numbers that can't be in the given cell from the array of possibilities.
//TODO: Implement search for number in same square
void find_cell_possibilities(struct cell matrix[9][9], int row, int column){

	//Check all the rows in the given column
	for(int i=0; i<9; i++){
		if(matrix[i][column].number!=0){
			int n = matrix[i][column].number;
			
			matrix[row][column].head.array[n-1].valid = 0;

			//Can't decrement "available" cause it causes underflow when this function is 
			//called more than once
			int x = 0;
			for (int i=0; i<9; i++){
				if (matrix[row][column].head.array[i].valid) x++;
			}
			matrix[row][column].head.available = x;

		}
	}


	//check all the columns in the given row
	for(int i=0; i<9; i++){
		if(matrix[row][i].number!=0){
			int n = matrix[row][i].number;
			if(matrix[row][column].head.array[n-1].valid){
				matrix[row][column].head.array[n-1].valid = 0;
				matrix[row][column].head.available -= 1;
			}
		}
	}


}


//Check if the number inserted in cell [row][column] conflicts with a number already placed
//general structure is similar to "find cell possibilities" function
int check_for_conflict(struct cell matrix[9][9], int row, int column){

	//Check the column by iterating over rows
	for(int i=0; i<9; i++){
		if(matrix[i][column].number == matrix[row][column].number && i!=row){
			//There's a conflict
			printf("Conflict in column\n");
			return 0;
		}
	}


	//Check the row by iterating over the columns
	for(int j=0; j<9; j++){
		if(matrix[row][j].number == matrix[row][column].number && j!=column){
			//There's a conflict
			printf("conflict in row\n");
			return 0;
		}
	}


	//Check the 3x3 square
	//To understand what position i am inside the square i check the remainder of the row and column %3
	//If the remainder is 0, i am on the leftmost column of a square or a highest row of a square, so i need to move right/down twice;
	//If the remainder is 1, i am in the second of the three rows or columns, so i need to check the previous and the next
	//If the remainder is 2, i am in the rightmost or lowest of columns/rows, so i need to check the previous 2
	//This means there are 9? possiblities. Can it be done with fewer?
	
	//TopLeft cell
	if(row%3 == 0 && column%3 == 0){
		for (int i = row; i < row+3; i++){
			for(int j = column; j < column+3; j++){
				if (matrix[i][j].number == matrix[row][column].number && i!=row && j!=column){
					printf("Found conflict in square 1\n");
					return 0;
				}
			}
		}
	}

	//TopMid cell
	else if(row%3 == 0 && column%3 == 1){
		for (int i = row; i < row+3; i++){
			for(int j = column-1; j < column+2; j++){
				if (matrix[i][j].number == matrix[row][column].number && i!=row && j!=column){
					printf("Found conflict in square 2 \n");
					return 0;
				}
			}
		}
	}

	//TopRight cell
	else if(row%3 == 0 && column%3 == 2){
		for (int i = row; i < row+3; i++){
			for(int j = column-2; j < column+1; j++){
				if (matrix[i][j].number == matrix[row][column].number && i!=row && j!=column){
					printf("Found conflict in square 3\n");
					return 0;
				}
			}
		}
	}

	//MidLeft cell
	else if(row%3 == 1 && column%3 == 0){
		for (int i = row-1; i < row+2; i++){
			for(int j = column; j < column+3; j++){
				if (matrix[i][j].number == matrix[row][column].number && i!=row && j!=column){
					printf("Found conflict in square 4\n");
					return 0;
				}
			}
		}
	}

	//Mid cell
	else if(row%3 == 1 && column%3 == 1){
		for (int i = row-1; i < row+2; i++){
			for(int j = column-1; j < column+2; j++){
				if (matrix[i][j].number == matrix[row][column].number && i!=row && j!=column) {
					printf("Found conflict in square 5\n");
					return 0;
				}
			}
		}
	}

	//MidRight cell
	else if(row%3 == 1 && column%3 == 2){
		for (int i = row-1; i < row+2; i++){
			for(int j = column-2; j < column+1; j++){
				if (matrix[i][j].number == matrix[row][column].number && i!=row && j!=column){
					printf("Found conflict in square 6\n");
					return 0;
				}
			}
		}
	}

	//BotLeft cell
	else if(row%3 == 2 && column%3 == 0){
		for (int i = row-2; i < row+1; i++){
			for(int j = column; j < column+3; j++){
				if (matrix[i][j].number == matrix[row][column].number && i!=row && j!=column){
					printf("Found conflict in square 7\n");
					return 0;
				}
			}
		}
	}

	//BotLeft cell
	else if(row%3 == 2 && column%3 == 1){
		for (int i = row-2; i < row+1; i++){
			for(int j = column-1; j < column+2; j++){
				if (matrix[i][j].number == matrix[row][column].number && i!=row && j!=column){
					printf("Found conflict in square 8\n");
					return 0;
				}
			}
		}
	}

	//BotLeft cell
	else if(row%3 == 2 && column%3 == 2){
		for (int i = row-2; i < row+1; i++){
			for(int j = column-2; j < column+1; j++){
				if (matrix[i][j].number == matrix[row][column].number && i!=row && j!=column){
					printf("Found conflict in square 9\n");
					return 0;
				}
			}
		}
	}

	return 1;
}



//TODO: implement solving of more complex cases before going for the backtracking alg
void solve(struct cell matrix[9][9], int solved){

	int starting = solved;

	//Until the sudoku is solved
	while(solved < 81){
		
		//Scan all cell to find one with only one possibility available
		for (int i=0; i<9; i++){
			for (int j=0; j<9; j++){
				//printf("[%d][%d] has %d available\n", i, j, matrix[i][j].head.available);

				if (matrix[i][j].head.available == 1){

					//If only one number is available, find it
					int n=0;
					while(!matrix[i][j].head.array[n].valid) n++;
					//printf("N is %d\n", n);

					matrix[i][j].head.array[n].valid = 0;

					matrix[i][j].head.available -= 1;

					matrix[i][j].number = n+1;
					solved ++;

					print_matrix(matrix);

				}
			}
		}
		
		if(starting == solved){
			printf("NEED TO CHANGE TO BACKTRACKING ALGORITHM\n");
			break;
		}

		//Calculate possibilities again
		for (int i=0; i<9; i++){
			for (int j=0; j<9; j++){ 
				if(matrix[i][j].number == 0) find_cell_possibilities(matrix, i, j);
			}
		}

		starting = solved;
	}


	//BACKTRACKING ALGORITHM
	//Starting from the first free cell, choose an available number and check if there's a
	//contradiction. If not move to next free cell and repeat process; If yes move back
	//to last cell that was filled with a guess and change the guess.


	//Put all the cells that have been guessed in an array so we can backtrack
	//We use a new struct bc its way easier maybe i actually don't know
	int r, c, array_head = 0;
	int size = 81 - solved;
	struct guess array_of_guesses[size];
	while(solved < 81){
		printf("%d\n", matrix[0][2].attempts);
		if(matrix[r][c].attempts >= 9){
			matrix[r][c].attempts = 0;
			array_head--;
			solved --;
			matrix[array_of_guesses[array_head].row][array_of_guesses[array_head].column].number = 0;
			printf("ARRAY HEAD AT %d\n", array_head);
			continue;
		}
		//I made it stupid so it restarts for every conflict, i really should use more data structures lol
		//It needs to "reload" the guesses that it has made till now
		for (int n=0; n<array_head; n++){
			r = array_of_guesses[n].row;
			c = array_of_guesses[n].column;
			matrix[r][c].number = array_of_guesses[n].number_guessed;
		}


		for (int i=0; i<9; i++){
			for (int j=0; j<9; j++){
				
				if(matrix[i][j].number == 0){	//Cell is empty and has to be guessed
					int attempts = matrix[i][j].attempts;
					int number_guess = matrix[i][j].head.array[attempts].number;

					//while(!curr.array[number_guess].valid && number_guess<10){ //Find the first currently available number as a guess
					//	number_guess++;
					//}	

					//if(number_guess == 9){
					//	printf("Unsolvable\n");
					//	return;
					//}

					array_of_guesses[array_head].number_guessed = number_guess;
					array_of_guesses[array_head].row = i;
					array_of_guesses[array_head].column = j;
					array_head++; 

					matrix[i][j].number = number_guess;
					matrix[i][j].attempts++;
					solved++;
					printf("Guessed %d in [%d][%d]\n", number_guess, i, j);
					print_matrix(matrix);
					//sleep(1);

					if(check_for_conflict(matrix, i, j) == 1){ //No Conflicts
						//Do nothing i guess
						printf("It seems ok\n");
					}
				
					if(check_for_conflict(matrix, i, j) == 0){ //There's a conflict
						//Need to go back and in the loop without restarting? fuck
						//This is hacky as fuck
						//printf("[%d][%d] guess (%d) is wrong panic AAAAAAAAAAAA\n\n\n", i, j, matrix[i][j].number);
						//sleep(1);
						matrix[i][j].number = 0;
						r=i, c=j;
						solved -= 1;
						array_head -= 1;
						i=9;
						j=9;
						break;
					}

				}

			}
		}

	}

	printf("%d\n", solved);
}









int main(){
	struct cell matrix[9][9];


	int solved = take_input(matrix);
	printf("Solved %d/81 cells\n", solved);
	print_matrix(matrix);

	//Find the possibilities for every cell
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

	printf("DONE\n");
}