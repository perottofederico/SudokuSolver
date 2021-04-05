//100904082052680300864200910010049806498300701607010093086035209509002130030497008
//100070030830600000002900608600004907090000050307500004203009100000002043040080009
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct possibility{
	int number;
	int valid;
};

struct possibilityArray{
	struct possibility array[9];
	int available;
};

struct cell{
	int number;
	struct possibilityArray head;
};



//Debug function to print the numbers that are possible for a given cell
void printArrayOfPossibility(struct cell cell, int row, int column){
	printf("POSSIBILITIES FOR [%d][%d]: ", row+1, column+1);
	for(int i=0; i<9; i++){
		if(cell.head.array[i].valid) printf("%d", cell.head.array[i].number);
	}
	printf("; Available: %d\n", cell.head.available);
}

//Function that takes a string and builds the matrix and the relative cells and info for it.
//TODO: Sanitize the input and make sure it only works with a 81 numbers long input.
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
	printf("++++++++++++++++++++++++++++++++++++++++++++++++\n");
}


//Function that checks all the numbers in the same row and column of the given cell and
//it removes the numbers that can't be in the given cell from the array of possibilities.
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


void solve(struct cell matrix[9][9], int solved){

	int starting = solved;

	//Until the sudoku is solved
	while(solved < 81){
		printf("SOLVED ARE %d\n", solved);
		
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