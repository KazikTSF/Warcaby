#pragma once
#include <string>
#include "Move.h"

class Board;

class Printer {
public:
    static void init(Board* boardInit) { board = boardInit; }
    static void printBoard();
    static void printPossibleMoves(const std::vector<Move>& possibleMoves);
    static std::string whitePawn;
    static std::string whiteQueen;
    static std::string blackPawn;
    static std::string blackQueen;
    static Board* board;
private:
    static void printEvenRow(int& i);
    static void printOddRow(int& i);

};
