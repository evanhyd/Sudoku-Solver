#include "Sudoku.h"
#include <algorithm>
#include <cctype>
#include <cassert>
#include <numeric>

int Sudoku::kGridToRowTable[kGridSize] = {};
int Sudoku::kGridToColumnTable[kGridSize] = {};
int Sudoku::kGridToBlockTable[kGridSize] = {};
int Sudoku::kBlockRangeTable[kGridLen][kGridLen] = {};

void Sudoku::InitTable()
{
    for (int i = 0; i < kGridSize; ++i)
    {
        kGridToRowTable[i] = i / kGridLen;
        kGridToColumnTable[i] = i % kGridLen;
        kGridToBlockTable[i] = kGridToRowTable[i] / 3 * 3 + kGridToColumnTable[i] / 3;

        int block_num = kGridToBlockTable[i];
        int starting_grid = block_num / 3 * 3 * kGridLen + block_num % 3 * 3;

        for (int offset = 0; offset < kGridLen; ++offset)
        {
            kBlockRangeTable[block_num][offset] = starting_grid + offset / 3 * kGridLen + offset % 3;
        }
    }
}

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
    for (int i = 0; i < kGridSize; ++i)
    {
        if(isdigit(new_board[i])) this->PutNum(i, new_board[i] - '0');
    }
}

bool Sudoku::Solve()
{
    return this->DFS();
}



void Sudoku::Deduce()
{
    while (true)
    {
        bool updated = false;

        for (int i = 0; i < kGridSize; ++i)
        {
            if (board_[i] != kEmptyGrid) continue;

            //check same row
            for (int row = kGridToRowTable[i], column = 0, grid = row * kGridLen + column; column < kGridLen; ++column, ++grid)
            {

            }
        }



        if (!updated) break;
    }
}


bool Sudoku::DFS()
{
    if (empty_grid_ == 0) return true;

    for (int i = 0; i < kGridSize; ++i)
    {
        if (board_[i] != kEmptyGrid) continue;

        U32 moves = GetMove(i);

        while (moves)
        {
            int num = GetLSTBit(moves);

            PutNum(i, num);
            if (this->DFS()) return true;
            UndoPutNum(i);

            moves = PopBit(moves);
        }
    }

    return false;
}

Sudoku::U32 Sudoku::GetMove(int grid)
{
    int row = kGridToRowTable[grid];
    int column = kGridToColumnTable[grid];
    int block = kGridToBlockTable[grid];

    //obtain possible moves
    U32 moves = ~(row_num_record_[row] | column_num_record_[column] | block_num_record_[block]) & 0b1111111110;

    return moves;
}

void Sudoku::PutNum(int grid, int num)
{
    board_[grid] = num;

    row_num_record_[kGridToRowTable[grid]] = FlipBit(row_num_record_[kGridToRowTable[grid]], num);
    column_num_record_[kGridToColumnTable[grid]] = FlipBit(column_num_record_[kGridToColumnTable[grid]], num);
    block_num_record_[kGridToBlockTable[grid]] = FlipBit(block_num_record_[kGridToBlockTable[grid]], num);

    --empty_grid_;
}

void Sudoku::UndoPutNum(int grid)
{
    ++empty_grid_;

    row_num_record_[kGridToRowTable[grid]] = FlipBit(row_num_record_[kGridToRowTable[grid]], board_[grid]);
    column_num_record_[kGridToColumnTable[grid]] = FlipBit(column_num_record_[kGridToColumnTable[grid]], board_[grid]);
    block_num_record_[kGridToBlockTable[grid]] = FlipBit(block_num_record_[kGridToBlockTable[grid]], board_[grid]);

    board_[grid] = kEmptyGrid;
}



std::ostream& operator<<(std::ostream& output, const Sudoku& game)
{
    for (int i = 0, grid = 0; i < Sudoku::kGridLen; ++i)
    {
        if (i % 3 == 0) output << "  -----------------------\n";
        for (int j = 0; j < Sudoku::kGridLen; ++j, ++grid)
        {
            if (j % 3 == 0) output << " |";

            if (game.board_[grid] == Sudoku::kEmptyGrid) output << " .";
            else output << ' ' << game.board_[grid];
        }

        output << " |\n";
    }
    output << "  -----------------------\n";

    output << "Empty grid remains: " << game.empty_grid_ << '\n';

    return output;
}