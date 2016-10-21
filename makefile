sudoku: sudokuMain.o sudokuParser.o sudokuSolver.o
	gcc sudokuMain.o sudokuParser.o sudokuSolver.o -lm -o sudoku -std=c99 -Wall -Wextra -g

sudokuSolver.o: sudokuSolver.c sudoku.h
	gcc -c sudokuSolver.c -std=c99 -Wall -Wextra -g

sudokuParser.o: sudokuParser.c sudoku.h	
	gcc -c sudokuParser.c -std=c99 -Wall -Wextra -g

sudokuMain.o: sudokuMain.c sudoku.h
	gcc -c sudokuMain.c -std=c99 -Wall -Wextra -g
	
clean:
	rm *.o sudoku
