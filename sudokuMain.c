//Student ID: 140009228
//3 March 2016
//C Programming 2
//sudokuMain.c

#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

int main(int argc, char** argv) {
	
	//if user provides a problem file and solution file
	if(argc == 3) {
		FILE* prob = fopen(argv[1], "r");
		FILE* sol = fopen(argv[2], "r");
	
		if(prob == 0) {
			printf("Error opening killer sudoku problem file.\n");
			exit(0);
		} else if (sol == 0) {
			printf("Error opening killer sudoku solution file.\n");
			exit(0);
		} 
		
		//check for valid problem
		int validProb = parseProblem(prob);
		if(!validProb) {
			printf("INVALIDPROBLEM\n");
			fclose(sol);
			exit(0);
		} 
	
		//check for complete and correct solution
		int validSol = parseSolution(sol);
		if (validSol == 0) {
			printf("INVALIDSOL\n");
			exit(0);
		} else if (validSol == -1) {
			printf("INCOMPLETE\n");
			exit(0);
		} 
		
		free(grid);
		free(cageArr);
		printf("SOLVED\n");
	
	//if user only provides one argument
	} else if(argc == 2) {
		FILE* prob = fopen(argv[1], "r");
		
		//check for valid problem
		int validProb = parseProblem(prob);
		if(!validProb) {
			printf("INVALIDPROBLEM\n");
			exit(0);
		} 
		
		solveCell(0,0);
		free(grid);
		free(cageArr);
	} else {
		printf("Error -- invalid arguments (two to check validity, one to solve)\n");
	}
}