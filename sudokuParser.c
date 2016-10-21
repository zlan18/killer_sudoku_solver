#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sudoku.h"

//returns a cell instance with the given x- and y-coordinates as attributes
struct cell* makeCell(int x, int y, int val) {
	struct cell* c = malloc(sizeof(struct cell)); 
	c->xCoord = x;
	c->yCoord = y;
	c->value = val;
	return c;
}

//returns a cage instance with a given number of cells, their sum value
//and an array of the cells involved
struct cage* makeCage(int n, int s) {
	struct cage* c = malloc(sizeof(struct cage));
	c->numCells = n;
	c->sum = s;
	c->coords = malloc(sizeof(struct cell) * n);
	return c;
}

//creates a 2-D integer array of size 'n' by 'n'
//assumes sudoku boards are always square
void initializeGrid(FILE* f) {
	fscanf(f, "%d", &gridSize);
	grid = malloc(sizeof(struct cell) * gridSize * gridSize);
	
	//initializes each cell value to -1
	for(int y = 0; y < gridSize; y++) {
		struct cell* gridRow = malloc(sizeof(struct cell) * gridSize);
		for(int x = 0; x < gridSize; x++) {
			gridRow[x] = *makeCell(x, y, -1);
		}
		grid[y] = gridRow;
	}
	
	//sets size of cageArr to number of cages indicated in file
	fscanf(f, "%d", &numCages);
	cageArr = malloc(sizeof(struct cage) * numCages);
}

//fills grid with values read in from solution file row by row
int fillGrid(FILE* f) {
	for(int y = 0; y < gridSize; y++) {
		for(int x = 0; x < gridSize; x++) {
			int val;
			fscanf(f, "%d", &val);
			if(val == 0) {	//if a square not yet filled in is found
				return -1;
			} else if (val < 0 || val > gridSize) { // if a square contains an invalid number
				return 0;
			} else {
				grid[x][y].value = val;
			}
		}
	}
	return 1;
}

//parses solved cages for validity - add up to correct sum, no repeated numbers
int parseCages() {
	//for every cage in the cageArr array
	for(int i = 0; i < numCages; i++) {
		struct cage curr;
		curr = cageArr[i];
		int count = 0;
		
		//checks for repeated numbers within cage
		//and adds the value of each cell to a counter
		for(int j = 0; j < curr.numCells; j++) {
			struct cell c = *curr.coords[j];
			count += c.value;
			for(int z = (j + 1); z < curr.numCells; z++) {
				struct cell cm = *curr.coords[z];
				if(c.value == cm.value) {
					return 0;
				}
			}
		}
		//cage adds up to the wrong sum
		if(count != curr.sum) {
			return 0;
		}
	} 
	return 1;
}

//checks a given solved subbox of the grid for repeated numbers
//x and y are the coordinates of the uppermost leftmost cell of the subbox
int parseSubBox(int x, int y, int size) {
	int vals[size*size];
	int counter = 0;
	
	//adds value of each cell in subbox to vals array
	for(int yp = y; yp < (y + size); yp++) {
		for(int xp = x; xp < (x + size); xp++) {
			vals[counter] = grid[xp][yp].value;
			counter++;
		}
	}
	
	//checks vals array for repeated numbers
	for(int i = 0; i < (size * size); i++) {
		for (int j = (i + 1); j < (size * size); j++) {
			if (vals[i] == vals[j]) {
				return 0;
			}
		}
	}
	return 1;
}

//calls 'parseSubBox' function on every subbox in the solved grid
//assumes each subbox is a square and has a size equal to the square root of the grid size
int parseSubBoxes() {
	int boxSize = (int) sqrt(gridSize);
	for(int x = 0; x < gridSize; x += boxSize) {
		for(int y = 0; y < gridSize; y += boxSize) {
			int validBox = parseSubBox(x, y, boxSize);
			if(!validBox) {
				return 0;
			}
		}
	}
	return 1;
}

//checks a given column for repeated numbers
//compares each value to all the values left in the column to follow
int parseCol(int x) {
	for(int yp = 0; yp < gridSize; yp++) {
		for(int mp = (yp + 1); mp < gridSize; mp++) {
			if(grid[x][yp].value == grid[x][mp].value) {
				return 0;
			}
		}
	}
	return 1;
}

//checks a given row for repeated numbers
//compares each value to all the values left in the row to follow
int parseRow(int y) {
	for(int xp = 0; xp < gridSize; xp++) {
		for(int mp = (xp + 1); mp < gridSize; mp++) {
			if(grid[xp][y].value == grid[mp][y].value) {
				return 0;
			}
		}
	}
	return 1;
}

//checks solved rows and columns for repeated numbers
int parseRowsAndCols() {
	for(int xp = 0; xp < gridSize; xp++) {
		int colValid = parseCol(xp);
		if(!colValid) {
			return 0;
		} 
	}
	
	for(int yp = 0; yp < gridSize; yp++) {
		int rowValid = parseRow(yp);
		if(!rowValid) {
			return 0;
		}
	}
	return 1;
}

//checks an empty killer sudoku grid as formatted in a problem file for validity
//ensures all squares are part of one and only one cage
int parseProblem(FILE* f) {	
	initializeGrid(f);
	
	//for every cage in file
	for(int k = 0; k < numCages; k++) {
		int numCells;
		int sum;
		
		//scan its indicated number of cells and sum value
		fscanf(f, "%d %d", &numCells, &sum);
		
		//fill cageArr with cage instance
		cageArr[k] = *makeCage(numCells, sum);
		
		//for every cell that is part of this cage
		for(int j = 0; j < numCells; j++) {
			int xCoord;
			int yCoord;
			
			//scan its x- and y-coordinates
			fscanf(f, "%d,%d", &xCoord, &yCoord);
			
			//since all cell values were initialized to -1
			//change from -1 to 0 to indicate this cell is now part of a cage
			if(grid[xCoord][yCoord].value == -1) {
				grid[xCoord][yCoord].value = 0;
				cageArr[k].coords[j] = &grid[xCoord][yCoord];
				grid[xCoord][yCoord].cg = &cageArr[k];
			} else {
				return 0;
			}
		}
	}
	
	//checks if any cells are not included in a cage
	for(int xp = 0; xp < gridSize; xp++) {
		for(int yp = 0; yp < gridSize; yp++) {
			if(grid[xp][yp].value == -1) {
				return 0;
			}
		}
	}
	fclose(f);
	return 1;
}

//checks for a complete and correct solution as read in from a solution file
int parseSolution(FILE* f) {
	int validNums = fillGrid(f);
	fclose(f);
	if(validNums != 1) {
		return validNums;
	} 
	
	int cagesValid = parseCages();		
	if (!cagesValid) {
		return 0;
	} 
	
	int boxesValid = parseSubBoxes();		
	if(!boxesValid) {
		return 0;
	}
	
	int rowsAndColsValid = parseRowsAndCols();
	if (!rowsAndColsValid) {
		return 0;
	}
	
	return 1;
}
