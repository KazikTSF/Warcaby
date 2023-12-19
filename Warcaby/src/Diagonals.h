#pragma once
#include "Move.h"

class Diagonals {
public:
    static bool bWhiteMove;
    static void setBoard(int board[32]);
    static void changeMove();
    static std::vector<Move> possibleDiagonals(const int pos, const int pawnType, const bool reversed);
    static int queenDiagonal(std::vector<Move>& diagonals);
    static std::vector<Move> possibleDiagonalsBoth(int pos, int pawnType);
    
private:
    static int board[32];
    static void findInDirection(std::vector<Move>& diagonalsInDirection, MoveDirection direction, int startPos, int pawnType, bool reversed);
    
};
