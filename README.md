# Sudoku-Solver

# Data Structure:  
2D matrix to represent the sudoku  
Arrays of bit masks to represent the number appear in the row/column/block

# Algorithm:  
Using DFS backtrack to search for the solution  
Greedily fill out the grid with the least move options to prune down the subtree  
Fast move generation by utilizing bits maniplation  
  
  
# To Do List:  
Update the data structure to dancing list  
Add puzzle generator  
Support custom sudoku dimension  
Machine Learning may be?  
