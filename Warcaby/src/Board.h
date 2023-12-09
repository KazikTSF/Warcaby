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
    
    std::vector<Move> possibleDiagonals(int pos) const;
    std::vector<Move> queenDiagonal(const std::vector<Move>& startDiagonals) const;
    
    std::vector<Move> findNormalMoves(int pos, bool bIsQueen) const;
    std::vector<Move> findJumps(int pos, bool bIsQueen);
    
    void makeMove(const Move& move);
    void unmakeLastMove();
    void findInDirection(std::vector<Move>& diagonalsInDirection, MoveDirection direction, int startPos) const;

    void printEvenRow(int& i, int reversedBoard[]) const;
    void printOddRow(int& i, int reversedBoard[]) const;
};
