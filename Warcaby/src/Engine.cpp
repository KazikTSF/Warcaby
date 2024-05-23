#include "Engine.h"

#include <algorithm>
#include <iostream>

#include "Printer.h"

int Engine::evaluate(const Board& board) {
    int eval = 0;
    for(int i = 0; i < 32; i++) {
        if(board.getBoard()[i] != 0) {
            const int valueModifier = board.getBoard()[i] > 0 ? -1 : 1;
            switch (board.getBoard()[i]) {
            case 1:
            case -1:
                eval+=PAWN_VALUE*valueModifier;
                break;
            case 2:
            case -2:
                eval+=QUEEN_VALUE*valueModifier;
                break;
            default:
                break;
            }
            if(board.getBoard()[i] > 0)
                eval+=valueModifier*(5-(i/8+1));
            else
                eval+=valueModifier*(i/8+1);
        }
    }
    return eval;
}
int Engine::mini(Board& board, int& alpha, int& beta, const int depth) {
    if(depth == 0)
        return evaluate(board);
    if(board.isLost())
        return board.isWhiteMove() ? INT_MIN : INT_MAX;
    int score = INT_MAX;
    for(Move& move : board.getPossibleMoves()) {
        board.makeMove(move, true);
        score = std::min(score, maxi(board, alpha, beta, depth-1));
        board.unmakeLastMove(true);
        if(score <= alpha)
            return alpha;
        if(score < beta)
            beta = score;
    }
    return score;
}
int Engine::maxi(Board& board, int& alpha, int& beta, const int depth) {
    if(depth == 0)
        return evaluate(board);
    if(board.isLost())
        return board.isWhiteMove() ? INT_MIN : INT_MAX;
    int score = INT_MIN;
    for(Move& move : board.getPossibleMoves()) {
        board.makeMove(move, true);
        score = std::max(score, mini(board, alpha, beta, depth-1));
        board.unmakeLastMove(true);
        if(score >= beta)
            return beta;
        if(score > alpha)
            alpha = score;
    }
    return score;
}
Move Engine::bestMove(Board& board, const int depth) {
    std::vector<Move> possibleMoves = board.getPossibleMoves();
    int mEvalIndex = 0, mEval = INT_MIN;
    int i = 0;
    for(Move& move : possibleMoves) {
        int alpha = INT_MIN, beta = INT_MAX;
        board.makeMove(move, true);
        const int eval = mini(board, alpha, beta, depth-1);
        board.unmakeLastMove(true);
        if(eval > mEval) {
            mEval = eval;
            mEvalIndex = i;
        }
        i++;
    }
    return possibleMoves[mEvalIndex];
}
