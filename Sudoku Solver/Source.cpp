#include <iostream>
#include <fstream>
#include <cstring>
#include "Sudoku.h"

int main(int argc, char* argv[])
{
    if (argc < 2) return 1;

    Sudoku game;

    if (strcmp(argv[1], "-s") == 0 && argc == 4)
    {
        std::ifstream input_file(argv[2]);
        if (!input_file)
        {
            std::cerr << "Couldn't open the input file\n";
            return 1;
        }

        std::ofstream output_file(argv[3]);
        if(!output_file)
        {
            std::cerr << "Couldn't create the output file\n";
            return 1;
        }

        int count = 0;
        std::string line;
        while (std::getline(input_file, line))
        {
            ++count;
            line.pop_back();
            game.SetPuzzle(line);

            std::clog << "Solving puzzle #" << count << '\n';
            if (game.Solve()) output_file << game << ",\n";
            else std::cout << "Unsolvable puzzle detected\n";
        }

        input_file.close();
        output_file.close();
    }
    else if (strcmp(argv[1], "-g") == 0 && argc == 5)
    {
        if (argc < 5) return 1;
        int difficulty = std::stoi(argv[2]);
        int num = std::stoi(argv[3]);
        std::ofstream output_file(argv[4]);
        if (!output_file)
        {
            std::cerr << "Couldn't create the output file\n";
            return 1;
        }

        for(int count = 1; count <= num; ++count)
        {
            std::clog << "Generating sudoku puzzles: " << count <<'/' << num << '\n';
            game.Shuffle(difficulty);
            output_file << game << ",\n";
        }

        output_file.close();
    }
    else if (strcmp(argv[1], "-v") == 0 && argc == 3)
    {
        std::ifstream input_file(argv[2]);
        if (!input_file)
        {
            std::cerr << "Couldn't open the input file\n";
            return 1;
        }

        int count = 0;
        std::string line;
        while (std::getline(input_file, line))
        {
            ++count;
            line.pop_back();
            game.SetPuzzle(line);
            
            std::cout << "Puzzle #" << count << '\n';
            std::cout << game << '\n';
        }

        input_file.close();
    }
    else return 1;

    return 0;
}