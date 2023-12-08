#include "Game.h"

Game::Game(const bool bUnicode) {
    board = new Board(bUnicode);
    board->generateMoves();
    board->printPossibleMoves();
}
