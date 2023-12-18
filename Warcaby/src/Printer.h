#pragma once
#include <string>
#include "Move.h"

class Printer {
public:
    static void printBoard(int board[32]);
    static void printPossibleMoves(const std::vector<Move>& possibleMoves);
    static std::string whitePawn;
    static std::string whiteQueen;
    static std::string blackPawn;
    static std::string blackQueen;
private:
    static void printEvenRow(int& i, int board[32]);
    static void printOddRow(int& i, int board[32]);

};
