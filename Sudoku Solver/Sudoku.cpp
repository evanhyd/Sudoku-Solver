#include "Sudoku.h"
#include <algorithm>
#include <random>
#include <cctype>
#include <cassert>


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
Sudoku::Sudoku(int clue) : Sudoku()
{
    this->Shuffle(clue);
}
Sudoku::Sudoku(const std::string& new_board) : Sudoku()
{
    SetPuzzle(new_board);
}

void Sudoku::SetPuzzle(const std::string& puzzle_str)
{
    assert(puzzle_str.size() == kGridSize);

    empty_grid_ = kGridSize;
    std::fill(row_num_record_, row_num_record_ + kGridLen, 0);
    std::fill(column_num_record_, column_num_record_ + kGridLen, 0);
    std::fill(block_num_record_, block_num_record_ + kGridLen, 0);
    
    auto letter = puzzle_str.begin();
    for (int i = 0; i < kGridLen; ++i)
    {
        for (int j = 0; j < kGridLen; ++j)
        {
            if ('1' <= *letter && *letter <= '9') this->PutNum(i, j, *letter - '0');
            else board_[i][j] = 0;
            ++letter;
        }
    }
}

void Sudoku::Shuffle(int clue)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 2);

    //reset to the original
    for (int i = 0; i < kGridLen; ++i)
    {
        for (int j = 0; j < kGridLen; ++j)
        {
            board_[i][j] = kCompletedPuzzle[i][j];
        }
    }

    //swap rows
    for (int i = 0; i < kGridLen; ++i)
    {
        int dest_row = i / 3 * 3 + dist(rng);

        //swap two rows
        std::swap(board_[i], board_[dest_row]);
    }

    //swap row groups
    for (int i = 0; i < 3; ++i)
    {
        int srce_row = i * 3;
        int dest_row = dist(rng) * 3;

        //swap two row groups
        for (int offset = 0; offset < 3; ++offset)
        {
            std::swap(board_[srce_row + offset], board_[dest_row + offset]);
        }
    }


    //swap columns
    for (int j = 0; j < kGridLen; ++j)
    {
        int dest_column = j / 3 * 3 + dist(rng);

        //swap two columns
        for (int i = 0; i < kGridLen; ++i)
        {
            std::swap(board_[i][j], board_[i][dest_column]);
        }
    }


    //swap column groups
    for (int j = 0; j < 3; ++j)
    {
        int srce_column = j * 3;
        int column_group_to_swap = dist(rng) * 3;

        //swap two row groups
        for (int offset = 0; offset < 3; ++offset)
        {
            for (int i = 0; i < kGridLen; ++i)
            {
                std::swap(board_[i][srce_column + offset], board_[i][column_group_to_swap + offset]);
            }
        }
    }

    //update number counts
    std::fill(row_num_record_, row_num_record_ + kGridLen, 0);
    std::fill(column_num_record_, column_num_record_ + kGridLen, 0);
    std::fill(block_num_record_, block_num_record_ + kGridLen, 0);
    empty_grid_ = kGridSize;

    for (int i = 0; i < kGridLen; ++i)
    {
        for (int j = 0; j < kGridLen; ++j)
        {
            PutNum(i, j, board_[i][j]);
        }
    }

    this->ReduceClue(clue);
}

bool Sudoku::Solve()
{
    return this->DFS(0);
}



//private

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

void Sudoku::ReduceClue(int clue)
{
    clue = std::clamp(clue, 0, kGridSize);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, kGridLen - 1);

    while ((kGridSize - empty_grid_) > clue)
    {
        int i = dist(rng);
        int j = dist(rng);

        if (board_[i][j] == kEmptyGrid) continue;

        UndoPutNum(i, j);

        assert(this->IsSolvable());
    }
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

bool Sudoku::IsSolvable()
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
        bool res = this->IsSolvable();
        UndoPutNum(best_i, best_j);

        if (res) return true;
    }

    return false;
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

    output << "Grids: " << (Sudoku::kGridSize - game.empty_grid_) <<'/' << Sudoku::kGridSize << '\n';

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

    return output;
}