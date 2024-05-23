#include "Engine.h"

#include <algorithm>
#include <random>

#include "Printer.h"

int Engine::evaluate(const Board& board) {
    int eval = 0;
    for(int i = 0; i < 32; i++) {
        if(board.getBoard()[i] != 0) {
            int valueModifier = board.getBoard()[i] > 0 ? -1 : 1;
            if(board.isWhiteMove())
                valueModifier *=-1;
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
            //im dalej jest pionek w strone przeciwnika tym wiecej punktow otrzymuje
            if(board.getBoard()[i] > 0)
                eval+=valueModifier*(5-(i/8+1));
            else
                eval+=valueModifier*(i/8+1);
        }
    }
    return eval;
}
int Engine::mini(Board& board, int& alpha, int& beta, const int depth) { //ocena posuniecia dla silnika
    if(board.isLost())
        return board.isWhiteMove() ? INT_MAX : INT_MIN;
    if(depth == 0)
        return -evaluate(board);
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
int Engine::maxi(Board& board, int& alpha, int& beta, const int depth) { //ocena posuniecia dla gracza
    if(board.isLost())
        return board.isWhiteMove() ? INT_MAX : INT_MIN;
    if(depth == 0)
        return evaluate(board);
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
    if(board.getMoveHistory().size() == 1) {
        std::random_device rd;
        std::uniform_int_distribution<> dist(1, possibleMoves.size()-2);
        return possibleMoves[dist(rd)];
    }
    for(auto& move : possibleMoves) {
        int alpha = INT_MIN, beta = INT_MAX;
        board.makeMove(move, true);
        move.eval = mini(board, alpha, beta, depth-1);
        board.unmakeLastMove(true);
    }
    
    int max = INT_MIN, maxIndex = 0;
    for(int i = 0; i < possibleMoves.size(); i++) {
        if(possibleMoves[i].eval > max) {
            max = possibleMoves[i].eval;
            maxIndex = i;
        }
    }
    return possibleMoves[maxIndex];
}