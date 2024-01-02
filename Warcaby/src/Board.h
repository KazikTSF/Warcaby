#pragma once
#include <string>
#include <vector>
#include "Move.h"

struct MoveHistory {
    Move move;
    std::vector<int> capturedPawns;
};

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
        if(moveHistory.empty())
            return *new Move(-1, -1, 0, MoveType::NONE, MoveDirection::NONE, {});
        return moveHistory.back().move;
    }
    std::vector<MoveHistory> getMoveHistory() const { return moveHistory; }
private:
    void generateMoves();
    int* board = new int[32];
    std::vector<MoveHistory> moveHistory;
    std::vector<Move> possibleMoves;
    bool bWhiteMove = true;
    std::vector<Move> findNormalMoves(std::vector<Move> diagonals, bool bIsQueen) const;
    std::vector<Move> findPawnJumps(int pos, int pawnType);
    static std::vector<Move> findLongestJumps(const std::vector<Move>& jumps);
};
