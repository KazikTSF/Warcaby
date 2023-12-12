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
        board[i] = -1;
    for(int i = 12; i < 20; i++)
        board[i] = 0;
    for(int i = 20; i < 32; i++)
        board[i] = 1;
    board[18] = -2;
    
    printBoard();
}
bool Board::isMoveLegal(const Move& move) const {
    for(const MoveHistory& m : moves) {
        Move temp = m.move;
        if(temp == move)
            return true;
    }
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
            auto jumpsT = findJumps(i, board[i]);
            std::vector<MoveHistory> jumps;
            jumps.reserve(jumpsT.size());
            for(Move m : jumpsT) {
                jumps.emplace_back(m, std::vector<int>());
            }
            if(!jumps.empty()) {
                if(!bJumpsPossible)
                    moves.clear();
                bJumpsPossible = true;
                for(auto j : jumps)
                    moves.push_back(j);
            }
            if(!bJumpsPossible) {
                std::vector<Move> diagonals = possibleDiagonals(i, piece, false);
                findNormalMoves(diagonals, bIsQueen);
                for(auto j : jumps)
                    moves.push_back(j);
            }
            
        }
    }
}

std::vector<Move> Board::findJumps(int pos, int pawnType) {
    std::vector<Move> jumps;
    std::vector<Move> diagonals = possibleDiagonalsBoth(pos, pawnType);
    for(Move m : diagonals) {
        if(bWhiteMove ? board[m.getEndPos()] < 0 : board[m.getEndPos()] > 0) {
            std::vector<Move> candidates = possibleDiagonalsBoth(m.getEndPos(), pawnType);
            int n = candidates.size();
            for(int i = 0; i < n; i++) {
                Move c = candidates.at(i);
                const bool bBothUp = c.getEndPos()-c.getStartPos() < 0 && m.getEndPos()-m.getStartPos() < 0 ? true : false;
                const bool bBothDown = c.getEndPos()-c.getStartPos() > 0 && m.getEndPos()-m.getStartPos() > 0 ? true : false; 
                if(!(bBothDown || bBothUp) || c.getMoveDirection() != m.getMoveDirection()) {
                    candidates.erase(std::find(candidates.begin(), candidates.end(), c));
                    n = candidates.size();
                    i--;
                }
                
            }
            if(candidates.empty())
                continue;
            if (board[candidates.at(0).getEndPos()] == 0) { //kandydat moze byc tylko jeden
                jumps.emplace_back(m.getStartPos(), candidates.at(0).getEndPos(), pawnType, MoveType::JUMP, MoveDirection::JUMP);
                jumps.at(jumps.size()-1).addCaptured(m.getEndPos());
            }
        }
    }
    if(jumps.empty())
        return {};
    for(Move jump : jumps) {
        makeMove(jump);
        printBoard();
        bWhiteMove = !bWhiteMove;
        findJumps(jump.getEndPos(), pawnType);
        unmakeLastMove();
        bWhiteMove = !bWhiteMove;
    }
    return jumps;
}

void Board::makeMove(const Move& move) {
    
    std::vector<int> capturedPawns;
    board[move.getStartPos()] = 0;
    if(move.getMoveType() == MoveType::JUMP) {
        for(const int captured : move.getCapturedPositions()) {
            capturedPawns.push_back(board[captured]);
            board[captured] = 0;
        }
    }
    board[move.getEndPos()] = move.getPawnType();
    bWhiteMove = !bWhiteMove;
    const MoveHistory history = {move, capturedPawns};
    moves.push_back(history);
}
void Board::unmakeLastMove() {
    const MoveHistory lastMove = moves.at(moves.size()-1);
    moves.erase(moves.end()-1);
    for(int i = 0 ; i < lastMove.capturedPawns.size(); i++) {
        board[lastMove.move.getCapturedPositions().at(i)] = lastMove.capturedPawns.at(i);
    }
    board[lastMove.move.getStartPos()] = lastMove.move.getPawnType();
    board[lastMove.move.getEndPos()] = 0;
    bWhiteMove = !bWhiteMove;
}

void Board::findInDirection(std::vector<Move>& diagonalsInDirection, MoveDirection direction, int startPos, int pawnType) const {
    size_t n = diagonalsInDirection.size();
    for(int i = 0; i < n; i++) {
        auto candidates = possibleDiagonals(diagonalsInDirection.at(i).getEndPos(), pawnType, false);
        for(Move move : candidates) {
            if(move.getMoveDirection() == direction) {
                n++;
                diagonalsInDirection.emplace_back(startPos, move.getEndPos(), pawnType, move.getMoveType(), move.getMoveDirection());
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
    findInDirection(left, MoveDirection::LEFT, left.at(0).getStartPos(), bWhiteMove ? 2 : -2);
    findInDirection(right, MoveDirection::RIGHT, right.at(0).getStartPos(), bWhiteMove ? 2 : -2);
    left.insert(left.end(), right.begin(), right.end());
    return left;
}
std::vector<Move> Board::possibleDiagonals(const int pos, const int pawnType, const bool reversed) const {
    std::vector<Move> diagonals;
    const int row=pos/4;
    const bool bLastColumn = (pos+1)%4==0 && (pos+1)%8 != 0;
    const bool bFirstColumn = (pos+1)%4==1 && (pos+1)%8 != 1;
    if(row % 2 == 1) {
        if(!bFirstColumn) {
            if(bWhiteMove && !reversed)
                diagonals.emplace_back(pos, pos-5, pawnType, MoveType::NORMAL, MoveDirection::LEFT);
            else
                diagonals.emplace_back(pos, pos+3, pawnType, MoveType::NORMAL, MoveDirection::LEFT);
        }
        if(bWhiteMove && !reversed)
            diagonals.emplace_back(pos, pos-4, pawnType, MoveType::NORMAL, MoveDirection::RIGHT);
        else
            diagonals.emplace_back(pos, pos+4, pawnType, MoveType::NORMAL, MoveDirection::RIGHT);
    }
    else {
        if(!bLastColumn) {
            if(bWhiteMove && !reversed)
                diagonals.emplace_back(pos, pos-3, pawnType, MoveType::NORMAL, MoveDirection::RIGHT);
            else
                diagonals.emplace_back(pos, pos+5, pawnType, MoveType::NORMAL, MoveDirection::RIGHT);
        }
        if(bWhiteMove && !reversed)
            diagonals.emplace_back(pos, pos-4, pawnType, MoveType::NORMAL, MoveDirection::LEFT);
        else
            diagonals.emplace_back(pos, pos+4, pawnType, MoveType::NORMAL, MoveDirection::LEFT);
    }
    return diagonals;
}
void Board::findNormalMoves(std::vector<Move>& diagonals, bool bIsQueen) const {
    if(bIsQueen) {
        diagonals = queenDiagonal(diagonals);
        bool bEndQueenMoveLeft = false, bEndQueenMoveRight = false;;
        int startRight = 0;
        for(int i = 0; i < diagonals.size(); i++) {
            if(diagonals.at(i).getMoveDirection() == MoveDirection::RIGHT) {
                startRight = i;
                break;
            }
        }
        for(int i = 0; i < diagonals.size(); i++) {
            Move move = diagonals.at(i);
            if(i < startRight) {
                if(bEndQueenMoveLeft) {
                    diagonals.erase(std::find(diagonals.begin(), diagonals.end(), move));
                    i--;
                    startRight--;
                    continue;
                }
                if(board[move.getEndPos()] != 0) {
                    bEndQueenMoveLeft = true;
                    diagonals.erase(std::find(diagonals.begin(), diagonals.end(), move));
                    i--;
                    startRight--;
                }
            }
            else {
                if(bEndQueenMoveRight) {
                    diagonals.erase(std::find(diagonals.begin(), diagonals.end(), move));
                    i--;
                    startRight--;
                    continue;
                }
                if(board[move.getEndPos()] != 0) {
                    bEndQueenMoveRight = true;
                    diagonals.erase(std::find(diagonals.begin(), diagonals.end(), move));
                    i--;
                    startRight--;
                }
            }
        }
    }
    else {
        auto n = diagonals.size();
        for(int i = 0; i < n; i++) {
            Move move = diagonals.at(i);
            const auto index = std::find(diagonals.begin(), diagonals.end(), move);
            if(board[move.getEndPos()] != 0) {
                diagonals.erase(index);
                n=diagonals.size();
                i--;
            }
        }
    }
}

std::vector<Move> Board::possibleDiagonalsBoth(int pos, int pawnType) const {
    std::vector<Move> temp1 = possibleDiagonals(pos, pawnType, false);
    std::vector<Move> temp2 = possibleDiagonals(pos, pawnType, true);
    temp1.insert(temp1.end(), temp2.begin(), temp2.end());
    return temp1;
}

void Board::printEvenRow(int& i) const {
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

void Board::printOddRow(int& i) const {
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
void Board::printBoard() const {
    const std::string lineSeparator = "  +---+---+---+---+---+---+---+---+";
    std::cout << lineSeparator << std::endl;
    for(int i = -1; i < 28;) {
        if((8-(i+1)/4) % 2 == 1)
            printEvenRow(i);
        else
            printOddRow(i);
        std::cout << std::endl << lineSeparator << std::endl;
    }
    std::cout << "    a   b   c   d   e   f   g   h" << std::endl;
}

void Board::printPossibleMoves() const {
    for(int i = 0; i < moves.size(); i++) {
        Move move = moves.at(i).move;
        if(i != 0) {
            if(move.getStartPos() != moves.at(i-1).move.getStartPos())
                std::cout << std::endl;
        }
        printf("%d - %d;  ", move.getStartPos()+1, move.getEndPos()+1);
    }
}



