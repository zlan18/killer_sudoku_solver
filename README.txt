# killer_sudoku_solver

A killer sudoku problem is like a regular sudoku problem, but it's empty from the start and cells group together to form cages. The values in each cell of the cage must add up to a particular sum for that cage. No cage contains the same number twice. 

If 2 arguments are provided (both filenames), it's assumed that the first is a killer sudoku problem and the second is a solution. Both files are parsed and either “INVALIDPROBLEM”, “INVALIDSOL”, “INCOMPLETE”, or “SOLVED” are output. If only one argument is provided (a filename), it's assumed that it's a killer sudoku problem to be solved. It's then solved recursively, printing out the final solution if one exists.
