#include "Game.h"

#include <iostream>

#include "Engine.h"

Game::Game(const bool bUnicode) {
    board = new Board(bUnicode);
    board->generateMoves();
    board->printPossibleMoves();
    bool bContinue = true;
    while(bContinue) {
        std::string move;
        std::cout << "\nWprowadz ruch: ";
        std::cin >> move;
        try {
            board->makeMove(board->findMove(Move::parseMove(move, board->isWhiteMove())));
        } catch(std::invalid_argument& e) {
            std::cout << "Ruch nie jest legalny, Podaj inny ruch\n";
            continue;
        }
        board->generateMoves();
        board->makeMove(Engine::bestMove(*board, !board->isWhiteMove(), 0));
        board->generateMoves();
        board->printBoard();
        board->printPossibleMoves();
        bContinue = !board->isLost();
    }
}
