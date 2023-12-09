#include "Board.h"

#include <complex>
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
    board[10] = 2;
    printBoard();
}
bool Board::isMoveLegal(const Move& move) const {
    if(std::find(moves.begin(), moves.end(), move) != moves.end())
        return true;
    return false;
}

void Board::generateMoves() {
    bool bJumpsPossible = false;
    for(int i = 0; i < 32; i++) {
        const int piece = board[i];
        const bool bIsQueen = abs(piece)==2 ? true : false;
        if(piece == 0)
            continue;
        if(bWhiteMove ? piece > 0 : piece < 0) {
            auto jumps = findJumps(i, bIsQueen);
            if(!jumps.empty()) {
                if(!bJumpsPossible)
                    moves.clear();
                bJumpsPossible = true;
                moves.insert(moves.end(), jumps.begin(), jumps.end());
            }
            if(!bJumpsPossible) {
                auto normal = findNormalMoves(i, bIsQueen);
                moves.insert(moves.end(), normal.begin(), normal.end());
            }
            
        }
    }
}

std::vector<Move> Board::findJumps(int pos, bool bIsQueen) {
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

void Board::findInDirection(std::vector<Move>& diagonalsInDirection, MoveDirection direction, int startPos) const {
    size_t n = diagonalsInDirection.size();
    for(unsigned i = 0; i < n; i++) {
        auto candidates = possibleDiagonals(diagonalsInDirection.at(i).getEndPos());
        for(Move move : candidates) {
            if(move.getMoveDirection() == direction) {
                n++;
                diagonalsInDirection.emplace_back(startPos, move.getEndPos(), move.getPawnType(), move.getMoveType(), move.getMoveDirection());
                break;
            }
        }
    }
}

std::vector<Move> Board::queenDiagonal(const std::vector<Move>& diagonals) const {
    std::vector<Move> left;
    std::vector<Move> right;
    for(Move move : diagonals) {
        if(move.getMoveDirection() == MoveDirection::LEFT)
            left.push_back(move);
        else if(move.getMoveDirection() == MoveDirection::RIGHT)
            right.push_back(move);
    }
    findInDirection(left, MoveDirection::LEFT, left.at(0).getStartPos());
    findInDirection(right, MoveDirection::RIGHT, right.at(0).getStartPos());
    left.insert(left.end(), right.begin(), right.end());
    return left;
}
std::vector<Move> Board::possibleDiagonals(int pos) const {
    std::vector<Move> diagonals;
    const int row=pos/4;
    const bool bFirstColumn = (pos+1)%4==0 && (pos+1)%8 != 0;
    const bool bLastColumn = (pos+1)%4==1 && (pos+1)%8 != 1;
    if(row % 2 == 0) {
        if(!bFirstColumn) {
            if(bWhiteMove)
                diagonals.emplace_back(pos, pos+5, board[pos], MoveType::NORMAL, MoveDirection::LEFT);
            else
                diagonals.emplace_back(pos, pos-4, board[pos], MoveType::NORMAL, MoveDirection::RIGHT);
        }
        if(bWhiteMove)
            diagonals.emplace_back(pos, pos+4, board[pos], MoveType::NORMAL, MoveDirection::RIGHT);
        else
            diagonals.emplace_back(pos, pos-3, board[pos], MoveType::NORMAL, MoveDirection::LEFT);
    }
    else {
        if(!bLastColumn) {
            if(bWhiteMove)
                diagonals.emplace_back(pos, pos+3, board[pos], MoveType::NORMAL, MoveDirection::RIGHT);
            else
                diagonals.emplace_back(pos, pos-5, board[pos], MoveType::NORMAL, MoveDirection::RIGHT);
        }
        if(bWhiteMove)
            diagonals.emplace_back(pos, pos+4, board[pos], MoveType::NORMAL, MoveDirection::LEFT);
        else
            diagonals.emplace_back(pos, pos-4, board[pos], MoveType::NORMAL, MoveDirection::LEFT);
    }
    return diagonals;
}
std::vector<Move> Board::findNormalMoves(int pos, bool bIsQueen) const {
    std::vector<Move> normalMoves = possibleDiagonals(pos);
    if(bIsQueen)
        normalMoves = queenDiagonal(normalMoves);
    for(Move move : normalMoves) {
        if(board[move.getEndPos()] != 0)
            normalMoves.erase(std::find(normalMoves.begin(), normalMoves.end(), move));
        else if(bIsQueen)
            break;
    }
    return normalMoves;
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

void Board::printPossibleMoves() const {
    for(unsigned i = 0; i < moves.size(); i++) {
        Move move = moves.at(i);
        if(i != 0) {
            if(move.getStartPos() != moves.at(i-1).getStartPos())
                std::cout << std::endl;
        }
        printf("%d - %d ", move.getStartPos()+1, move.getEndPos()+1);
    }
}