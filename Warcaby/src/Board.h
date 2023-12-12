#pragma once
#include <string>
#include <vector>

#include "Move.h"

class Board {
public:
    explicit Board(bool);
    ~Board() {delete [] board;}
    
    bool isMoveLegal(const Move&) const;
    void generateMoves();

    void printBoard() const;
    void printPossibleMoves() const;
private:
    std::string whitePawn = "\xE2\x9B\x82";
    std::string whiteQueen = "\xE2\x9B\x83";
    std::string blackPawn = "\xE2\x9B\x80";
    std::string blackQueen = "\xE2\x9B\x81";
    
    int* board = new int[32];
    std::vector<Move> moves;
    
    bool bWhiteMove = true;
    
    std::vector<Move> possibleDiagonals(const int pos, const bool reverse) const;
    std::vector<Move> queenDiagonal(const std::vector<Move>& startDiagonals) const;
    std::vector<Move> possibleDiagonalsBoth(int pos) const;

    void findNormalMoves(std::vector<Move>& diagonals, bool bIsQueen) const;

    void findJumps(int pos, int pawnType, std::vector<Move>& jumps) const;
    
    void makeMove(const Move& move);
    void unmakeLastMove();
    void findInDirection(std::vector<Move>& diagonalsInDirection, MoveDirection direction, int startPos) const;

    void printEvenRow(int& i) const;
    void printOddRow(int& i) const;
};
