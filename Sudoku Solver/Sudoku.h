#pragma once
#include <iostream>
#include <fstream>
#include <string>

class Sudoku
{
public:
    using U32 = unsigned int;
    static constexpr int kEmptyGrid = 0;
    static constexpr int kGridLen = 9;
    static constexpr int kGridSize = kGridLen * kGridLen;
    static constexpr U32 kFilterMoveMask = 0b1111111110;

private:
    static constexpr int kGridToBlockTable[kGridLen][kGridLen] = 
    {
        {0, 0, 0, 1, 1, 1, 2, 2, 2},
        {0, 0, 0, 1, 1, 1, 2, 2, 2},
        {0, 0, 0, 1, 1, 1, 2, 2, 2},
        {3, 3, 3, 4, 4, 4, 5, 5, 5},
        {3, 3, 3, 4, 4, 4, 5, 5, 5},
        {3, 3, 3, 4, 4, 4, 5, 5, 5},
        {6, 6, 6, 7, 7, 7, 8, 8, 8},
        {6, 6, 6, 7, 7, 7, 8, 8, 8},
        {6, 6, 6, 7, 7, 7, 8, 8, 8}
    };

    static constexpr int kCompletedPuzzle[kGridLen][kGridLen] =
    {
       {1, 2, 3, 4, 5, 6, 7, 8, 9},
       {4, 5, 6, 7, 8, 9, 1, 2, 3},
       {7, 8, 9, 1, 2, 3, 4, 5, 6},
       {2, 3, 1, 6, 7, 4, 8, 9, 5},
       {8, 7, 5, 9, 1, 2, 3, 6, 4},
       {6, 9, 4, 5, 3, 8, 2, 1, 7},
       {3, 1, 7, 2, 6, 5, 9, 4, 8},
       {5, 4, 2, 8, 9, 7, 6, 3, 1},
       {9, 6, 8, 3, 4, 1, 5, 7, 2}
    };

    static U32 PopBit(U32 num);
    static U32 FlipBit(U32 num, int bit);
    static int GetLSTBit(U32 num);

private:
    int board_[kGridLen][kGridLen];
    int empty_grid_;
    U32 row_num_record_[kGridLen];
    U32 column_num_record_[kGridLen];
    U32 block_num_record_[kGridLen];

public:
    Sudoku();
    explicit Sudoku(int clue);
    explicit Sudoku(const std::string& new_board);

    void SetPuzzle(const std::string& puzzle_str);
    void Shuffle(int clue);
    bool Solve();

private:
    U32 GetMove(int i, int j);
    void PutNum(int i, int j, int num);
    void UndoPutNum(int i, int j);
    void ReduceClue(int clue);

    bool DFS(int depth);
    bool IsSolvable();


    friend std::ostream& operator<<(std::ostream& output, const Sudoku& game);
    friend std::ofstream& operator<<(std::ofstream& output, const Sudoku& game);
};

