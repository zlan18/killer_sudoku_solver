#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sudoku.h"

//checks a given column for repeated numbers
//compares each value to all the values left in the column to follow
//will only compare up to as far as the column as been filled in (ignores empty cells)
int checkCol(int x) {
	for(int y = 0; y < gridSize; y++) {
		for(int mp = (y + 1); mp < gridSize; mp++) {
			if(grid[x][y].value == 0) {
				return 1;
			} else if(grid[x][y].value == grid[x][mp].value) {
				return 0;
			}
		}
	}
	return 1;
}

//checks a given row for repeated numbers
//compares each value to all te values left in the row to follow
//will only compare as far as the row has been filled in (ignores empty cells)
int checkRow(int y) {
	for(int x = 0; x < gridSize; x++) {
		for(int mp = (x + 1); mp < gridSize; mp++) {
			if(grid[x][y].value == 0) {
				return 1;
			} else if(grid[x][y].value == grid[mp][y].value) {
				return 0;
			}
		}
	}
	return 1;
}

//checks a given subbox of the grid for repeated numbers
//x and y are the coordinates of a cell within the subbox
int checkBox(int x, int y) {
	int size = (int) sqrt(gridSize);
	int vals[size*size];
	int counter = 0;
	
	int xp, yp;
	
	//finds the uppermost leftmost coordinates of the subbox
	if(x % size == 0) {
		xp = x;
	} else {
		xp = (x/size) * size;
	}
	if(y % size == 0) {
		yp = y;
	} else {
		yp = (y/size) * size;
	}
	
	//adds value of each cell in subbox to vals array
	for(int r = yp; r < (yp + size); r++) {
		for(int c = xp; c < (xp + size); c++) {
			vals[counter] = grid[c][r].value;
			counter++;
		}
	}
	
	//checks vals array for repeated numbers
	// will only compare as far as the subbox has been filled in (ignores empty cells)
	for(int i = 0; i < (size * size); i++) {
		for (int j = (i + 1); j < (size * size); j++) {
			if(vals[i] == 0) {
				return 1;
			} else if (vals[i] == vals[j]) {
				return 0;
			}
		}
	}
	return 1;
}

//checks a given cage for repeated numbers and a correct sum
//x and y are the coordinates of a cell within the cage
int checkCage(int x, int y) {
	//the cage which this cell is associated with
	struct cage c = *grid[x][y].cg;
	int count = 0;
	
	//checks for repeated numbers within cage
	//if a cell is not yet filled, make sure the count is still below the final sum
	for(int i = 0; i < c.numCells; i++) {
		struct cell cp = *c.coords[i];
		count += cp.value;	 //add the value of this cell to the count
		for(int k = (i + 1); k < c.numCells; k++) {
			struct cell cm = *c.coords[k];
			if(cm.value == 0) {
				if (count < c.sum) {
					return 1;
				} 
			} else if(cp.value == cm.value) {
				return 0;
			}
		}
	}
	
	//will only get here if cage is completely filled in 
	//checks if cage adds up to the wrong sum
	if(count != c.sum) {
		return 0;
	}
	
	return 1;
}

//prints out a simple grid of numbers to the console
void printSolution() {
	for(int r = 0; r < gridSize; r++) {
		for(int c = 0; c < gridSize; c++) {
			int val = grid[c][r].value;
			if(val == 0) {
				printf("_ ");
			} else {
				printf("%d ", val);
			}
		}
		printf("\n");
	}
} 

//recursive function to solve a killer sudoku grid
void solveCell(int x, int y) {
	
	//for all values 1 to 'n' for a grid of size 'n'
	for(int i = 1; i <= gridSize; i++) {
		
		//try a value
		grid[x][y].value = i;
		
		//if the solution is not invalid
		if(checkRow(y) && checkCol(x) && checkBox(x, y) && checkCage(x, y)) {
			if(x == (gridSize-1) && y == (gridSize-1)) {	//if the last cell was just filled in
				printSolution();	//print the solution
				exit(0);
			} else if(x == (gridSize-1)) {	//if the last cell of a row was just filled in
				solveCell(0, (y+1));	//start from the beginning of the next row
			} else {
				solveCell((x+1), y);	//move along the row
			}
		}
	}
	if(x == 0 && y == 0) {
		//all possible solutions have been exhausted
		printf("Problem cannot be solved.\n");
	} else {
		//all values for this cell do not make this solution valid
		//backtrack and try changing the value of a previous cell
		grid[x][y].value = 0;
	}
}
