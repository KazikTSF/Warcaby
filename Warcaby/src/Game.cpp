#include "Game.h"

#include <iostream>

#include "Engine.h"
#include "Printer.h"

Game::Game(const bool bUnicode) {
    int depth = 5;
    char choice = '\0';
    do {
        board = new Board(bUnicode);
        std::cout << "Czy gra ma sie toczyc pomiedzy 2 silnikami (t/n)?\n";
        do {
            std::cin >> choice;
        } while(choice != 't' && choice != 'n');
    
        Printer::printBoard();
        if(choice == 'n'){
            Printer::printPossibleMoves(board->getPossibleMoves());
        }
        bool bContinue = true;
        while(bContinue) {
            std::string move;
            if(choice == 'n'){
                std::cout << "\nWprowadz ruch: ";
                std::cin >> move;
                try {
                    board->makeMove(board->findMove(Move::parseMove(move, board->isWhiteMove())), true);
                } catch(std::invalid_argument& e) {
                    std::cout << "Ruch nie jest legalny, Podaj inny ruch\n";
                    continue;
                }
            }
            else {
                board->makeMove(Engine::bestMove(*board, 3), true);
            }
            Printer::printBoard();
            if(board->isLost())
                break;
            board->makeMove(Engine::bestMove(*board, 3), true);
            if(board->isLost())
                break;
            Printer::printBoard();
            if(choice == 'n'){
                Printer::printPossibleMoves(board->getPossibleMoves());
            }
       
        }
        std::cout << "Koniec gry\n";
        std::cout << "Wygrywają: " << (board->isWhiteMove() ? "czarne" : "biale") << std::endl;
        delete board;
        std::cout << "Czy chcesz zaczac kolejna gre? (t/n)\n";
        choice = '\0';
        do {
            std::cin >> choice;
        } while(choice != 't' && choice != 'n');
    } while (choice == 't');
}
