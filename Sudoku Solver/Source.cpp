#include <iostream>
#include "Sudoku.h"

int main()
{
    Sudoku::InitTable();

    Sudoku game =
    {
        "8....4..12.3.6..9.4.6..8.52.8.5.9........15....23....8...8..4....9..5..........6."
    };

    Sudoku answer =
    {
        "897254631253167894416938752681529347934781526572346918365812479149675283728493165"
    };

    std::cout << game << '\n';
    std::cout << answer << '\n';

    if (game.Solve()) std::cout << "Solved\n";
    else std::cout << "Error, failed to solve\n";

    std::cout << game << '\n';
}