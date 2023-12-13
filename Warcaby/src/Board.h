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
    struct MoveHistory {
        Move move;
        std::vector<int> capturedPawns;
        MoveHistory(const Move& m, const std::vector<int>& c) : move(m), capturedPawns(c) {}
    };
    std::string whitePawn = "\xE2\x9B\x82";
    std::string whiteQueen = "\xE2\x9B\x83";
    std::string blackPawn = "\xE2\x9B\x80";
    std::string blackQueen = "\xE2\x9B\x81";
    
    int* board = new int[32];
    std::vector<MoveHistory> moves;
    std::vector<Move> possibleMoves;
    bool bWhiteMove = true;
    
    std::vector<Move> possibleDiagonals(const int pos, const int pawnType, const bool reversed) const;
    std::vector<Move> queenDiagonal(const std::vector<Move>& startDiagonals) const;

    void findNormalMoves(std::vector<Move>& diagonals, bool bIsQueen) const;
    std::vector<Move> possibleDiagonalsBoth(int pos, int pawnType) const;
    std::vector<Move> findJumps(int pos, int pawnType, std::vector<int> captured);
    static std::vector<Move> findLongestJumps(const std::vector<Move>& jumps);
    void makeMove(const Move& move);
    void unmakeLastMove();
    void findInDirection(std::vector<Move>& diagonalsInDirection, MoveDirection direction, int startPos, int pawnType) const;

    void printEvenRow(int& i) const;
    void printOddRow(int& i) const;
};
