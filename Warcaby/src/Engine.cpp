#include "Engine.h"

#include <ctime>


Move Engine::bestMove(const Board& board, const bool bIsWhite, const int depth) {
    srand(time(nullptr));
    const int bestMoveIndex = rand() % board.getPossibleMoves().size();
    return board.getPossibleMoves().at(bestMoveIndex);
}
