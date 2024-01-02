#pragma once
#include "Board.h"

class Engine {
private:
    const static int pawnValue = 5;
    const static int queenValue = 15;
public:
    static int evaluate(const Board& board);
    static int mini(Board& board, int& alpha, int& beta, const int depth);
    static int maxi(Board& board, int& alpha, int& beta, const int depth);
    static Move bestMove(Board& board, const int depth);
};
