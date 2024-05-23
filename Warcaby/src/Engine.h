#pragma once
#include "Board.h"

class Engine {
private:
    static constexpr int PAWN_VALUE = 5;
    static constexpr int QUEEN_VALUE = 15;
public:
    static int evaluate(const Board& board);
    static int mini(Board& board, int& alpha, int& beta, const int depth);
    static int maxi(Board& board, int& alpha, int& beta, const int depth);
    static Move bestMove(Board& board, const int depth);
};
