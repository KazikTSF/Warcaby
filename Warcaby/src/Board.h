#pragma once
#include <string>
#include <vector>

#include "Move.h"

class Board {
public:
    explicit Board(bool);
    ~Board() {delete [] board;}
private:
    std::string whitePawn = "\xE2\x9B\x82";
    std::string whiteQueen = "\xE2\x9B\x83";
    std::string blackPawn = "\xE2\x9B\x80";
    std::string blackQueen = "\xE2\x9B\x81";
    
    int* board = new int[32];
    std::vector<Move> moves;
    
    bool bWhiteMove = true;
    
    bool isMoveLegal(const Move&) const;
    std::vector<int> possibleDiagonals(int, bool) const;
    void queenDiagonal(std::vector<int>& diagonals, int pos) const;
    
    std::vector<Move> findNormalMoves(int);
    std::vector<Move> findJumps(int);
    
    void makeMove(const Move&);
    void unmakeLastMove();
    
    void printEvenRow(int&, int[]) const;
    void printOddRow(int&, int[]) const;
    void printBoard() const;
};
