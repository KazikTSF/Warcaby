#pragma once
#include <string>
#include <vector>

#include "Move.h"

class Board {
public:
    explicit Board(bool);
    ~Board() {delete [] board;}
    
    void checkQueenDiagonal(std::vector<Move>& jumps, int& startReversed, int& startRight, int start,
                            int& stop) const;
    std::vector<Move> findQueenJumps(int pos, int pawnType);
    void makeMove(const Move& move, bool bGenerate);
    void unmakeLastMove(bool bGenerate);
    bool isWhiteMove() const { return bWhiteMove; }
    bool isLost() const;
    Move findMove(const Move& move) const;
    std::vector<Move> getPossibleMoves() const { return possibleMoves; }
    int* getBoard() const { return board; }
    Move getLastMove() const {
        if(moves.empty())
            return *new Move(-1, -1, 0, MoveType::NONE, MoveDirection::NONE, {});
        return moves.back().move;
    }

private:
    struct MoveHistory {
        Move move;
        std::vector<int> capturedPawns;
    };
    
    void generateMoves();
    int* board = new int[32];
    std::vector<MoveHistory> moves;
    std::vector<Move> possibleMoves;
    bool bWhiteMove = true;
    std::vector<Move> findNormalMoves(std::vector<Move> diagonals, bool bIsQueen) const;
    std::vector<Move> findPawnJumps(int pos, int pawnType);
    static std::vector<Move> findLongestJumps(const std::vector<Move>& jumps);
};
