#include "Sudoku.h"
#include <algorithm>
#include <cctype>
#include <cassert>
#include <numeric>


Sudoku::U32 Sudoku::PopBit(U32 num)
{
    return num & (num - 1);
}
Sudoku::U32 Sudoku::FlipBit(U32 num, int bit)
{
    return num ^ (1 << bit);
}
int Sudoku::GetLSTBit(U32 num)
{
    return std::popcount(num ^ (num - 1)) - 1;
}



Sudoku::Sudoku() : board_{}, empty_grid_(kGridSize), row_num_record_{}, column_num_record_{}, block_num_record_{}
{
    //empty
}
Sudoku::Sudoku(const std::string& new_board) : Sudoku()
{
    assert(new_board.size() == kGridSize);

    auto letter = new_board.begin();
    for (int i = 0; i < kGridLen; ++i)
    {
        for (int j = 0; j < kGridLen; ++j)
        {
            if(isdigit(*letter) && *letter != 0) this->PutNum(i, j, *letter - '0');
            ++letter;
        }
    }
}

bool Sudoku::Solve()
{
    return this->DFS(0);
}


bool Sudoku::DFS(int depth)
{
    if (empty_grid_ == 0) return true;

    int best_i = 0, best_j = 0;
    U32 best_moves = 0;
    int best_moves_options = 10;

    for (int i = 0; i < kGridLen; ++i)
    {
        for (int j = 0; j < kGridLen; ++j)
        {
            if (board_[i][j] != kEmptyGrid) continue;

            U32 moves = GetMove(i, j);
            if (moves == 0) return false;

            int move_options = std::popcount(moves);
            if (move_options < best_moves_options)
            {
                best_i = i;
                best_j = j;
                best_moves = moves;
                best_moves_options = move_options;
            }
        }
    }

    //choosing the grid with least options makes it extremely fast!!!
    for (; best_moves; best_moves = PopBit(best_moves))
    {
        int num = GetLSTBit(best_moves);

        PutNum(best_i, best_j, num);
        if (this->DFS(depth + 1)) return true;
        UndoPutNum(best_i, best_j);
    }

    return false;
}

Sudoku::U32 Sudoku::GetMove(int i, int j)
{
    //obtain possible moves
    return ~(row_num_record_[i] | column_num_record_[j] | block_num_record_[kGridToBlockTable[i][j]]) & kFilterMoveMask;
}

void Sudoku::PutNum(int i, int j, int num)
{
    board_[i][j] = num;

    row_num_record_[i] = FlipBit(row_num_record_[i], num);
    column_num_record_[j] = FlipBit(column_num_record_[j], num);

    int block = kGridToBlockTable[i][j];
    block_num_record_[block] = FlipBit(block_num_record_[block], num);

    --empty_grid_;
}

void Sudoku::UndoPutNum(int i, int j)
{
    ++empty_grid_;

    int num = board_[i][j];
    row_num_record_[i] = FlipBit(row_num_record_[i], num);
    column_num_record_[j] = FlipBit(column_num_record_[j], num);

    int block = kGridToBlockTable[i][j];
    block_num_record_[block] = FlipBit(block_num_record_[block], num);

    board_[i][j] = kEmptyGrid;
}



std::ostream& operator<<(std::ostream& output, const Sudoku& game)
{
    for (int i = 0; i < Sudoku::kGridLen; ++i)
    {
        if (i % 3 == 0) output << "  -----------------------\n";

        for (int j = 0; j < Sudoku::kGridLen; ++j)
        {
            if (j % 3 == 0) output << " |";

            if (game.board_[i][j] == Sudoku::kEmptyGrid) output << " .";
            else output << ' ' << game.board_[i][j];
        }

        output << " |\n";
    }
    output << "  -----------------------\n";

    output << "Empty grid remains: " << game.empty_grid_ << '\n';

    return output;
}
std::ofstream& operator<<(std::ofstream& output, const Sudoku& game)
{
    for (int i = 0; i < Sudoku::kGridLen; ++i)
    {
        for (int j = 0; j < Sudoku::kGridLen; ++j)
        {
            if (game.board_[i][j]) output << game.board_[i][j];
            else output << '.';
        }
    }

    output << '\n';

    return output;
}