#include "Game.h"

#include <iostream>

#include "Engine.h"
#include "Printer.h"
enum {
    depth = 7
};

Game::Game(const bool bUnicode) {
    char choice = '\0';
    do {
        board = new Board(bUnicode);
    
        Printer::printBoard(board);
        Printer::printPossibleMoves(board->getPossibleMoves());
        while(true) {
            std::string move;
            std::cout << "\nWprowadz ruch: ";
            std::cin >> move;
            try {
                board->makeMove(board->findMove(Move::parseMove(move, board->isWhiteMove())), true);
            } catch(std::invalid_argument&) {
                std::cout << "Ruch nie jest legalny, Podaj inny ruch\n";
                continue;
            }
            Printer::printBoard(board);
            std::cout << std::endl;
            if(board->isLost())
                break;
            board->makeMove(Engine::bestMove(*board, depth), true);
            if(board->isLost())
                break;
            Printer::printBoard(board);
            Printer::printPossibleMoves(board->getPossibleMoves());
       
        }
        Printer::printBoard(board);
        std::cout << "Koniec gry\n";
        std::cout << "Wygrywaja: " << (board->isWhiteMove() ? "czarne" : "biale") << std::endl;
        delete board;
        std::cout << "Czy chcesz zaczac kolejna gre? (t/n)\n";
        choice = '\0';
        do {
            std::cin >> choice;
        } while(choice != 't' && choice != 'n');
    } while (choice == 't');
}
