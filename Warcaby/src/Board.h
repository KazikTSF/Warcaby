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
    
    std::vector<Move> possibleDiagonals(int, bool) const;
    void queenDiagonal(std::vector<Move>&, int) const;
    
    std::vector<Move> findNormalMoves(int, bool) const;
    std::vector<Move> findJumps(int, bool);
    
    void makeMove(const Move&);
    void unmakeLastMove();
    
    void printEvenRow(int&, int[]) const;
    void printOddRow(int&, int[]) const;
};
