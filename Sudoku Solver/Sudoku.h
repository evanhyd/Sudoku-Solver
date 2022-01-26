#pragma once
#include <iostream>
#include <fstream>
#include <array>
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
    Sudoku(const std::string& new_board);

    bool Solve();

private:
    bool DFS(int depth);
    U32 GetMove(int i, int j);
    void PutNum(int i, int j, int num);
    void UndoPutNum(int i, int j);

    friend std::ostream& operator<<(std::ostream& output, const Sudoku& game);
    friend std::ofstream& operator<<(std::ofstream& output, const Sudoku& game);
};

