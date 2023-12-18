#pragma once
#include <string>
#include <vector>

#include "Move.h"

class Board {
public:
    explicit Board(bool);
    ~Board() {delete [] board;}
    
    void generateMoves();
    void checkQueenDiagonal(std::vector<Move>& jumps, int& startReversed, int& startRight, int start,
                            int& stop) const;
    std::vector<Move> findQueenJumps(int pos, int pawnType);
    void printBoard() const;
    void printPossibleMoves() const;
    void makeMove(const Move& move);
    void unmakeLastMove();
    bool isWhiteMove() const { return bWhiteMove; }
    bool isLost() const;
    Move findMove(const Move& move) const;
    std::vector<Move> getPossibleMoves() const { return possibleMoves; }

private:
    struct MoveHistory {
        Move move;
        std::vector<int> capturedPawns;
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
    int queenDiagonal(std::vector<Move>& diagonals) const;

    std::vector<Move> findNormalMoves(std::vector<Move> diagonals, bool bIsQueen) const;
    std::vector<Move> possibleDiagonalsBoth(int pos, int pawnType) const;
    std::vector<Move> findPawnJumps(int pos, int pawnType);
    static std::vector<Move> findLongestJumps(const std::vector<Move>& jumps);
    void findInDirection(std::vector<Move>& diagonalsInDirection, MoveDirection direction, int startPos, int pawnType, bool reversed) const;

    void printEvenRow(int& i) const;
    void printOddRow(int& i) const;

};
