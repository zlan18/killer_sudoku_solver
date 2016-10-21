#ifndef SUDOKU_H
#define SUDOKU_H

//global variables, features of the sudoku grid
struct cell** grid; 
int gridSize;
struct cage* cageArr;
int numCages;  

//a cage with a given number of cells and their coordinates and their sum value
struct cage {
	int numCells, sum;
	struct cell **coords;
};

//a cell of the grid with x- and y-coordinates
struct cell {
	int xCoord;
	int yCoord;
	struct cage* cg;
	int value;
};

int parseProblem(FILE* f);
int parseSolution(FILE* f);
void solveCell(int x, int y);

#endif
