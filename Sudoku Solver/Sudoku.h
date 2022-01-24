#pragma once
#include <iostream>
#include <array>
#include <string>

class Sudoku
{
public:
    using U32 = unsigned int;
    static constexpr int kEmptyGrid = 0;
    static constexpr int kGridLen = 9;
    static constexpr int kNumOptions = 10;
    static constexpr int kGridSize = kGridLen * kGridLen;

private:
    static int kGridToRowTable[kGridSize];
    static int kGridToColumnTable[kGridSize];
    static int kGridToBlockTable[kGridSize];
    static int kBlockRangeTable[kGridLen][kGridLen];
    
public:
    static void InitTable();

private:
    static U32 PopBit(U32 num);
    static U32 FlipBit(U32 num, int bit);
    static int GetLSTBit(U32 num);

private:
    int board_[kGridSize];
    int empty_grid_;
    U32 row_num_record_[kGridLen];
    U32 column_num_record_[kGridLen];
    U32 block_num_record_[kGridLen];

public:
    Sudoku();
    Sudoku(const std::string& new_board);

    bool Solve();

private:
    void Deduce();
    bool DFS();
    U32 GetMove(int grid);
    void PutNum(int grid, int num);
    void UndoPutNum(int grid);

    friend std::ostream& operator<<(std::ostream& output, const Sudoku& game);
};

