#pragma once
#include <string>
#include "Move.h"
#include "Board.h"
class Board;

class Printer {
public:
    static void printBoard(const Board* board);
    static void printPossibleMoves(const std::vector<Move>& possibleMoves);
    static void printHistory(const std::vector<MoveHistory>& moves);
    static std::string whitePawn;
    static std::string whiteQueen;
    static std::string blackPawn;
    static std::string blackQueen;
private:
    static void printEvenRow(int& i, const Board* board);
    static void printOddRow(int& i, const Board* board);

};
