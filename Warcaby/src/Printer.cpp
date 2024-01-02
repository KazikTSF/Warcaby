#include "Printer.h"

#include <iostream>
#include <windows.h>

#include "Board.h"
#include "Move.h"
    std::string Printer::whitePawn = "\xE2\x9B\x82";
    std::string Printer::whiteQueen = "\xE2\x9B\x83";
    std::string Printer::blackPawn = "\xE2\x9B\x80";
    std::string Printer::blackQueen = "\xE2\x9B\x81";
void Printer::printEvenRow(int& i, const Board* board) {
    std::cout << 8-(i+1)/4 << " ";
    for(int j = 0; j < 4; j++) {
        std::cout << "| ";
        if(board->getLastMove().getEndPos() == ++i)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        switch (board->getBoard()[i]) {
        case 0:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            std::cout << "  |   ";
            break;
        case 1:
            std::cout << whitePawn;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            std::cout << " |   ";
            break;
        case 2:
            std::cout << whiteQueen;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            std::cout << " |   ";
            break;
        case -1:
            std::cout << blackPawn;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            std::cout << " |   ";
            break;
        case -2:
            std::cout << blackQueen;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            std::cout << " |   ";
            break;
        default:
            throw std::invalid_argument("board[" + std::to_string(i) + "]");
        }
    }
    std::cout << "|";
}

void Printer::printOddRow(int& i, const Board* board) {
    std::cout << 8-(i+1)/4 << " |   ";
    for(int j = 0; j < 4; j++) {
        std::cout << "| ";
        if(board->getLastMove().getEndPos() == ++i)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        switch (board->getBoard()[i]) {
        case 0:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            std::cout << "  |   ";
            break;
        case 1:
            std::cout << whitePawn;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            std::cout << " |   ";
            break;
        case 2:
            std::cout << whiteQueen;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            std::cout << " |   ";
            break;
        case -1:
            std::cout << blackPawn;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            std::cout << " |   ";
            break;
        case -2:
            std::cout << blackQueen;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            std::cout << " |   ";
            break;
        default:
            throw std::invalid_argument("board[" + std::to_string(i) + "]");
        }
    }
}

void Printer::printBoard(const Board* board) {
    const std::string lineSeparator = "  +---+---+---+---+---+---+---+---+";
    std::cout << lineSeparator << std::endl;
    for(int i = -1; i < 28;) {
        if((8-(i+1)/4) % 2 == 1)
            printEvenRow(i, board);
        else
            printOddRow(i, board);
        std::cout << std::endl << lineSeparator << std::endl;
    }
    std::cout << "    a   b   c   d   e   f   g   h" << std::endl;
}

void Printer::printPossibleMoves(const std::vector<Move>& possibleMoves) {
    for(int i = 0; i < possibleMoves.size(); i++) {
        const Move& move = possibleMoves.at(i);
        if(i != 0) {
            if(move.getStartPos() != possibleMoves.at(i-1).getStartPos())
                std::cout << std::endl;
        }
        std::cout << move;
    }
}
void Printer::printHistory(const std::vector<MoveHistory>& moves) {
    std::cout << "history: ";
    for(const MoveHistory& move : moves) {
        std::cout << move.move << " ";
    }
    std::cout << std::endl;
}
