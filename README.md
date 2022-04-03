# Sudoku-Solver

# Data Structure:  
2D matrix to represent the sudoku  
Arrays of bit masks to represent the number appear in the row/column/block

# Algorithm:  
Using DFS backtrack to search for the solution  
Greedily fill out the grid with the least move options to prune down the subtree  
Fast move generation by using bitwise opreations to perform set intersect/unions.
  
  
# To Do List:  
<strike>Add puzzle generator</strike>  
Support custom sudoku dimension  
Update the data structure to dancing list  
Machine learning may be?  


# Command Arguments:
generate sudoku puzzles: -g [prefilled grid number] [puzzle numbers] [output file name]  
solve sudoku puzzles: -s [input file name] [output file name]  
view sudoku puzzles: -v [input file name]  

Notes:  
The sudoku puzzles are stored as CSV format, separated with a comma and a new line, examples:  
............87.................6.8.9.....2.....1.9...........83.....4...1239..6.7,  
............3.....5..1..643......7..7...5.......9......4...7.....25...9..78......,  
.69.......8.........3....67....87.4.........8..8..4.....4.....98..2..1...........,  
12..4..5...............8....9........3.5..7..............3.6...6...8.3.1......9.6,  
.8..5.....7.8..........3..92...7..4.....4......4.........5....7.5..3...1........3,  
  
Above are 5 different configurations for sudoku puzzles  
