# Sudoku-Solver

# Data Structure:  
2D matrix to represent the sudoku  
Arrays of bit masks to represent the number appear in the row/column/block

# Algorithm:  
Using DFS backtrack to search for the solution  
Greedily fill out the grid with the least move options to prune down the subtree  
Fast move generation by utilizing bits maniplation  
  
  
# To Do List:  
<strike>Add puzzle generator</strike>  
Support custom sudoku dimension  
Update the data structure to dancing list  
Machine learning may be?  


# Command Arguments:
generate sudoku puzzles: -g [difficulty] [puzzle numbers] [output file name]  
solve sudoku puzzles: -s [input file name] [output file name]  
view sudoku puzzles: -v [input file name]  
