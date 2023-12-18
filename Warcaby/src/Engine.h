#pragma once
#include "Board.h"

class Engine {
public:
    static float evaluate(const Board& board, const bool bIsWhite);
    static Move bestMove(const Board& board, const bool bIsWhite, const int depth);
};
