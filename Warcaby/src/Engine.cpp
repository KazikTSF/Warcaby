#include "Engine.h"

#include <algorithm>
#include <iostream>

int Engine::evaluate(const Board& board) {
    int eval = 0;
    for(int i = 0; i < 32; i++) {
        if(board.getBoard()[i] != 0) {
            int valueModifier;
            if(board.isWhiteMove())
                valueModifier = board.getBoard()[i] > 0 ? 1 : -1;
            else
                valueModifier = board.getBoard()[i] > 0 ? -1 : 1;
            switch (board.getBoard()[i]) {
            case 1:
            case -1:
                eval+=pawnValue*valueModifier;
                break;
            case 2:
            case -2:
                eval+=queenValue*valueModifier;
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
int Engine::mini(Board& board, const int depth) {
    if(depth == 0) return evaluate(board);
    int mini = INT_MAX;
    for(Move& move : board.getPossibleMoves()) {
        board.makeMove(move, true);
        mini = std::min(mini, maxi(board, depth-1));
        board.unmakeLastMove(true);
    }
    return mini;
}
int Engine::maxi(Board& board, const int depth) {
    if(depth == 0) return evaluate(board);
    int maxi = INT_MIN;
    for(Move& move : board.getPossibleMoves()) {
        board.makeMove(move, true);
        maxi = std::max(maxi, mini(board, depth-1));
        board.unmakeLastMove(true);
    }
    return maxi;
}
Move Engine::bestMove(Board& board, const int depth) {
    if(board.isLost()){
        auto temp =  Move(-1, -1, 0, MoveType::NONE, MoveDirection::NONE, {});
        temp.eval = board.isWhiteMove() ? INT_MIN : INT_MAX;
        return temp;
    }
    std::vector<Move> possibleMoves = board.getPossibleMoves();
    for(Move& move : possibleMoves) {
        board.makeMove(move, true);
        move.eval = mini(board, depth);
        board.unmakeLastMove(true);
    }
    //TODO debugging
    for(const auto& move : possibleMoves) {
        std::cout << move << " eval: " << -move.eval << std::endl;
    }
    std::sort( possibleMoves.begin( ), possibleMoves.end( ), [ ]( const Move& lhs, const Move& rhs )
{
   return lhs.eval < rhs.eval;
});
    return possibleMoves[0];
}
