#include "Board.h"
#include <iostream>

Board::Board(const bool bUnicode) {
    if(!bUnicode) {
        whitePawn = 'w';
        whiteQueen = 'W';
        blackPawn = 'b';
        blackQueen = 'B';
    }
    for(int i = 0; i < 12; i++)
        board[i] = 1;
    for(int i = 12; i < 20; i++)
        board[i] = 0;
    for(int i = 20; i < 32; i++)
        board[i] = -1;
    printBoard();
}
bool Board::isMoveLegal(const Move& move) const {
    if(std::find(moves.begin(), moves.end(), move) != moves.end())
        return true;
    return false;
}

std::vector<Move> Board::findJumps(int pos) {
    std::vector<Move> jumps;
    return jumps;
}

void Board::makeMove(const Move& move) {
    moves.push_back(move);
    board[move.getStartPos()] = 0;
    board[move.getEndPos()] = move.getPawnType();
    bWhiteMove = !bWhiteMove;
}
void Board::unmakeLastMove() {
    moves.erase(moves.end());
    bWhiteMove = !bWhiteMove;
}
void Board::queenDiagonal(std::vector<int>& diagonals, int pos) const {
    for (auto move : diagonals) {
    }
}
//TODO struct przechowujacy czy pionek idzie w lewo/prawo
std::vector<int> Board::possibleDiagonals(int pos, bool bQueen) const {
    std::vector<int> diagonals;
    const int row=pos/4;
    const bool bFirstColumn = pos%4==0 && pos%8 != 0;
    const bool bLastColumn = pos%4==1 && pos%8 != 1;
    if(row % 2 == 0) {
        if(!bFirstColumn) {
            if(bWhiteMove)
                diagonals.push_back(5);
            else
                diagonals.push_back(-4);
        }
        if(bWhiteMove)
            diagonals.push_back(4);
        else
            diagonals.push_back(-5);
    }
    else {
        if(!bLastColumn) {
            if(bWhiteMove)
                diagonals.push_back(3);
            else
                diagonals.push_back(-4);
        }
        if(bWhiteMove)
            diagonals.push_back(4);
        else
            diagonals.push_back(-3);
    }
    if(bQueen)
        queenDiagonal(diagonals, pos);
    return diagonals;
}

std::vector<Move> Board::findNormalMoves(int) {
    
}

void Board::printEvenRow(int& i, int reversedBoard[]) const {
    std::cout << 8-(i+1)/4 << " ";
    for(int j = 0; j < 4; j++) {
        std::cout << "| ";
        switch (reversedBoard[++i]) {
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

void Board::printOddRow(int& i, int reversedBoard[]) const {
    std::cout << 8-(i+1)/4 << " |   ";
    for(int j = 0; j < 4; j++) {
        std::cout << "| ";
        switch (reversedBoard[++i]) {
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
void Board::printBoard() const {
    const std::string lineSeparator = "  +---+---+---+---+---+---+---+---+";
    int temp[32];
    for(int i = 0; i < 32; i++)
        temp[i] = board[31-i];
    std::cout << lineSeparator << std::endl;
    for(int i = -1; i < 28;) {
        if((8-(i+1)/4) % 2 == 1)
            printEvenRow(i, temp);
        else
            printOddRow(i, temp);
        std::cout << std::endl << lineSeparator << std::endl;
    }
    std::cout << "    a   b   c   d   e   f   g   h" << std::endl;
}
