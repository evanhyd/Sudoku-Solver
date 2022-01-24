#include <iostream>
#include "Sudoku.h"

int main()
{
    Sudoku::InitTable();

    Sudoku game =
    {
        "7.2548.1338.192..6941...52....8147...2.376.51...92..34.7..51369.3628.147194...285"
    };

    Sudoku answer =
    {
        "762548913385192476941763528653814792429376851817925634278451369536289147194637285"
    };

    std::cout << game << '\n';
    std::cout << answer << '\n';

    if (game.Solve()) std::cout << "Solved\n";
    else std::cout << "Error, failed to solve\n";

    std::cout << game << '\n';
}