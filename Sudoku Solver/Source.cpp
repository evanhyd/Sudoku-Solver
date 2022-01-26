#include <iostream>
#include <fstream>
#include "Sudoku.h"

int main()
{
    std::ifstream input_file("sudoku_unsolved.csv");
    if (!input_file)
    {
        std::cout << "Unable to read sudoku_unsolved.csv\n";
        return EXIT_FAILURE;
    }

    std::ofstream output_file("sudoku_solved.csv");
    if (!output_file)
    {
        std::cout << "Unable to create sudoku_solved.csv\n";
        return EXIT_FAILURE;
    }


    for(int counter = 0; ; ++counter)
    {
        std::cout << "Test case #" << counter << '\n';

        std::string puzzle_str;
        std::getline(input_file, puzzle_str);

        if (puzzle_str.empty()) break;
        puzzle_str.pop_back();

        Sudoku game(puzzle_str);

        if (game.Solve())
        {
            std::cout << "completed\n";
            std::cout << game << '\n';
            output_file << game;
        }
        else
        {
            std::cout << game << '\n';
            std::cerr << "\aError, failed to solve\n";
            break;
        }
    }

    input_file.close();
    output_file.close();
}