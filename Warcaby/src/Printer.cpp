#include "Printer.h"

#include <iostream>

#include "Move.h"
    std::string Printer::whitePawn = "\xE2\x9B\x82";
    std::string Printer::whiteQueen = "\xE2\x9B\x83";
    std::string Printer::blackPawn = "\xE2\x9B\x80";
    std::string Printer::blackQueen = "\xE2\x9B\x81";
void Printer::printEvenRow(int& i, int board[32]) {
    std::cout << 8-(i+1)/4 << " ";
    for(int j = 0; j < 4; j++) {
        std::cout << "| ";
        switch (board[++i]) {
        case 0:
            std::cout << "  |   ";
            break;
        case 1:
            std::cout << whitePawn << " |   ";
            break;
        case 2:
            std::cout << whiteQueen << " |   ";
            break;
        case -1:
            std::cout << blackPawn << " |   ";
            break;
        case -2:
            std::cout << blackQueen << " |   ";
            break;
        default:
            throw std::invalid_argument("board[" + std::to_string(i) + "]");
        }
    }
    std::cout << "|";
}

void Printer::printOddRow(int& i, int board[32]) {
    std::cout << 8-(i+1)/4 << " |   ";
    for(int j = 0; j < 4; j++) {
        std::cout << "| ";
        switch (board[++i]) {
        case 0:
            std::cout << "  |   ";
            break;
        case 1:
            std::cout << whitePawn << " |   ";
            break;
        case 2:
            std::cout << whiteQueen << " |   ";
            break;
        case -1:
            std::cout << blackPawn << " |   ";
            break;
        case -2:
            std::cout << blackQueen << " |   ";
            break;
        default:
            throw std::invalid_argument("board[" + std::to_string(i) + "]");
        }
    }
}

void Printer::printBoard(int board[32]) {
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
        printf("%d - %d; ", move.getStartPos()+1, move.getEndPos()+1);
    }
}