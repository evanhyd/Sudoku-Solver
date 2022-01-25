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
        if(isdigit(new_board[i]) && new_board[i] != 0) this->PutNum(i, new_board[i] - '0');
    }
}

bool Sudoku::Solve()
{
    /*std::cout << "Deducing...\n";
    this->Deduce();

    if (empty_grid_ == 0) return true;*/

    //std::cout << "Brute Force DFS...\n";
    //std::cout << *this << '\n';

    return this->DFS(0);
}

void Sudoku::Deduce()
{
    while (true)
    {
        bool updated = false;

        for (int i = 0; i < kGridSize; ++i)
        {
            if (board_[i] != kEmptyGrid) continue;
            const U32 moves = this->GetMove(i);

            //check direct moves
            U32 direct_moves = moves;
            if (std::popcount(direct_moves) == 1)
            {
                int num = GetLSTBit(direct_moves);
                this->PutNum(i, num);
                updated = true;
                continue;
            }


            //check same row
            U32 row_moves = moves;
            for (int row = kGridToRowTable[i], column = 0; column < kGridLen; ++column)
            {
                int grid = row * kGridLen + column;

                //skip filled && same grid
                if (board_[grid] != kEmptyGrid || grid == i) continue;

                row_moves &= ~GetMove(grid);
            }
            row_moves &= kFilterMoveMask;
            if (std::popcount(row_moves) == 1)
            {
                int num = GetLSTBit(row_moves);
                this->PutNum(i, num);
                updated = true;
                continue;
            }


            //check same column
            U32 column_moves = moves;
            for (int row = 0, column = kGridToColumnTable[i]; row < kGridLen; ++row)
            {
                int grid = row * kGridLen + column;

                //skip filled && same grid
                if (board_[grid] != kEmptyGrid || grid == i) continue;

                column_moves &= ~GetMove(grid);
            }
            column_moves &= kFilterMoveMask;
            if (std::popcount(column_moves) == 1)
            {
                int num = GetLSTBit(column_moves);
                this->PutNum(i, num);
                updated = true;
                continue;
            }




            //check same block
            U32 block_moves = moves;
            for (int offset = 0; offset < kGridLen; ++offset)
            {
                //skip filled && same grid
                int grid = kBlockRangeTable[kGridToBlockTable[i]][offset];
                if (board_[grid] != kEmptyGrid || grid == i) continue;

                block_moves &= ~GetMove(grid);
            }
            block_moves &= kFilterMoveMask;
            if (std::popcount(block_moves) == 1)
            {
                int num = GetLSTBit(block_moves);
                this->PutNum(i, num);
                updated = true;
                continue;
            }
        }



        if (!updated) break;
    }
}

bool Sudoku::DFS(int depth)
{
    if (empty_grid_ == 0) return true;

    int best_grid = 0;
    U32 best_moves = 0;
    int best_move_options = 10;

    for (int i = 0; i < kGridSize; ++i)
    {
        if (board_[i] != kEmptyGrid) continue;

        U32 moves = GetMove(i);

        //no moves available!
        if (moves == 0) return false;

        int curr_move_options = std::popcount(moves);
        if (curr_move_options < best_move_options)
        {
            best_grid = i;
            best_moves = moves;
            best_move_options = curr_move_options;
        }
    }


    //choosing the grid with least options makes it extremely fast!!!
    while (best_moves)
    {
        int num = GetLSTBit(best_moves);

        PutNum(best_grid, num);
        if (this->DFS(depth + 1)) return true;
        UndoPutNum(best_grid);

        best_moves = PopBit(best_moves);
    }

    return false;
}

Sudoku::U32 Sudoku::GetMove(int grid)
{
    int row = kGridToRowTable[grid];
    int column = kGridToColumnTable[grid];
    int block = kGridToBlockTable[grid];

    //obtain possible moves
    U32 moves = ~(row_num_record_[row] | column_num_record_[column] | block_num_record_[block]) & kFilterMoveMask;

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
std::ofstream& operator<<(std::ofstream& output, const Sudoku& game)
{
    for (int i = 0; i < Sudoku::kGridSize; ++i)
    {
        if (game.board_[i]) output << game.board_[i];
        else output << '.';
    }
    output << '\n';

    return output;
}