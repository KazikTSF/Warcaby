#include "Board.h"
#include "Printer.h"
#include <algorithm>
#include <complex>
#include <iostream>
#include <windows.h>


Board::Board(const bool bUnicode) {
    if(!bUnicode) {
        Printer::whitePawn = 'w';
        Printer::whiteQueen = 'W';
        Printer::blackPawn = 'b';
        Printer::blackQueen = 'B';
    }
    for(int i = 0; i < 12; i++)
        board[i] = -1;
    for(int i = 12; i < 20; i++)
        board[i] = 0;
    for(int i = 20; i < 32; i++)
        board[i] = 1;
}
void Board::generateMoves() {
    possibleMoves.clear();
    bool bJumpsPossible = false;
    for(int i = 0; i < 32; i++) {
        const int piece = board[i];
        const bool bIsQueen = abs(piece)==2 ? true : false;
        if(piece == 0)
            continue;
        const bool bIsWhite = piece > 0 ? true : false;
        if((bWhiteMove && bIsWhite) || (!bWhiteMove && !bIsWhite)) {
            std::vector<Move> jumpsT;
            if(abs(piece) == 2)
                jumpsT = findQueenJumps(i, board[i]);
            else 
                jumpsT = findPawnJumps(i, board[i]);
            std::vector<Move> jumps;
            jumps.reserve(jumpsT.size());
            for(const Move& m : jumpsT)
                jumps.emplace_back(m);
            if(!jumps.empty()) {
                if(!bJumpsPossible)
                    possibleMoves.clear();
                bJumpsPossible = true;
                for(const auto& j : jumps)
                    possibleMoves.push_back(j);
            }
            if(!bJumpsPossible) {
                std::vector<Move> diagonals;
                if(bIsQueen)
                    diagonals = possibleDiagonalsBoth(i, piece);
                else
                    diagonals = possibleDiagonals(i, piece, false);
                diagonals = findNormalMoves(diagonals, bIsQueen);
                for(const auto& j : diagonals)
                    possibleMoves.push_back(j);
            }
        }
    }
    if(bJumpsPossible) {
        std::vector<Move> longestJumps;
        longestJumps.reserve(possibleMoves.size());
        for (auto& move : possibleMoves)
            longestJumps.push_back(move);
        longestJumps = findLongestJumps(longestJumps);
        possibleMoves.clear();
        for(Move& m : longestJumps)
            possibleMoves.emplace_back(m);
    }
}

void Board::checkQueenDiagonal(std::vector<Move>& jumps, int& startReversed, int& startRight, int start, int& stop) const {
    bool endDiagonalCapture = false;
    bool endDiagonalFriendly = false;
    int capturedPosInOneDiagonal = -1;
    if(stop == -1)
        stop = startReversed;
    for(int i = start; i < stop; i++) {
        const Move move = jumps.at(i);
        if(bWhiteMove ? board[move.getEndPos()] > 0 : board[move.getEndPos()] < 0) {
            endDiagonalFriendly = true;
        }
        if(endDiagonalFriendly) {
            jumps.erase(std::find(jumps.begin(), jumps.end(), move));
            i--;
            if(i < startReversed)
                startReversed--;
            if(i < startRight)
                startRight--;
            continue;
        }
        if(endDiagonalCapture) {
            if(board[move.getEndPos()] != 0) {
                jumps.erase(std::find(jumps.begin(), jumps.end(), move));
                i--;
                if(i < startReversed)
                    startReversed--;
                if(i < startRight)
                    startRight--;
                endDiagonalFriendly = true;
            }
        }
        else {
            jumps.erase(std::find(jumps.begin(), jumps.end(), move));
            i--;
            if(i < startReversed)
                startReversed--;
            if(i < startRight)
                startRight--;
            if((bWhiteMove ? board[move.getEndPos()] < 0 : board[move.getEndPos()] > 0) && i != stop-1) {
                capturedPosInOneDiagonal = move.getEndPos();
                endDiagonalCapture = true;
            }
        }
    }
    if(capturedPosInOneDiagonal == -1) {
        for(int i = start; i < stop; i++) {
            const auto it = std::find(jumps.begin(), jumps.end(), jumps.at(i));
            jumps.erase(it);
        }
        return;
    }
    if(capturedPosInOneDiagonal > 0) {
        for(int i = start; i < stop; i++) {
            jumps.at(i) = *new Move (jumps.at(i).getStartPos(), jumps.at(i).getEndPos(), jumps.at(i).getPawnType(), MoveType::JUMP, jumps.at(i).getMoveDirection());
            jumps.at(i).addCaptured(capturedPosInOneDiagonal);
        }
            
    }
}

std::vector<Move> Board::findQueenJumps(int pos, int pawnType) {
    std::vector<Move> jumps = possibleDiagonalsBoth(pos, pawnType);
    int startReversed = queenDiagonal(jumps), startRight = -1;
    for(int i = 0; i < startReversed; i++) {
        if(jumps.at(i).getMoveDirection() == MoveDirection::RIGHT) {
            startRight = i;
            break;
        }
    }
    if(startReversed != 0) {
        checkQueenDiagonal(jumps, startReversed, startRight, 0, startRight);
        if(startRight > -1)
            checkQueenDiagonal(jumps, startReversed, startRight, startRight, startReversed);
    }
    if(startReversed != jumps.size()) {
        startRight = -1;
        for(int i = startReversed; i < jumps.size(); i++) {
            if(jumps.at(i).getMoveDirection() == MoveDirection::RIGHT) {
                startRight = i;
                break;
            }
        }
        checkQueenDiagonal(jumps, startReversed, startRight, startReversed, startRight);
        startReversed = jumps.size();
        if(startRight > -1)
            checkQueenDiagonal(jumps, startReversed, startRight, startRight, startReversed);
    }
    if(jumps.empty())
        return {};
    std::vector<Move> temp;
    for (const auto& jump : jumps) {
        makeMove(jump);
        bWhiteMove = !bWhiteMove;
        std::vector<Move> nextJumps = findQueenJumps(jump.getEndPos(), pawnType);
        for(Move& m : nextJumps)
            m.addCaptured(jump.getCapturedPositions());
        temp.insert(temp.end(), nextJumps.begin(), nextJumps.end());
        unmakeLastMove();   
        bWhiteMove = !bWhiteMove;
    }
    jumps.insert(jumps.end(), temp.begin(), temp.end());
    for(Move& move : jumps) {
        move = *new Move(pos, move.getEndPos(), pawnType, MoveType::JUMP, MoveDirection::JUMP, move.getCapturedPositions());
    }
    return jumps;
}
std::vector<Move> Board::findPawnJumps(int pos, int pawnType) {
    std::vector<Move> jumps;
    const std::vector<Move> diagonals = possibleDiagonalsBoth(pos, pawnType);
    for(const Move& m : diagonals) {
        if(bWhiteMove ? board[m.getEndPos()] < 0 : board[m.getEndPos()] > 0) { //sprawdza czy bity pionek jest przeciwnego koloru
            std::vector<Move> candidates = possibleDiagonalsBoth(m.getEndPos(), pawnType);
            int n = candidates.size();
            for(int i = 0; i < n; i++) {
                Move c = candidates.at(i);
                const bool bBothUp = c.getEndPos()-c.getStartPos() < 0 && m.getEndPos()-m.getStartPos() < 0 ? true : false;
                const bool bBothDown = c.getEndPos()-c.getStartPos() > 0 && m.getEndPos()-m.getStartPos() > 0 ? true : false; 
                if (!(bBothDown || bBothUp) || c.getMoveDirection() != m.getMoveDirection()) {
                    //jesli oba ruchy nie sa w tym samym kierunku, lub kierunek ruchu kandydata jest inny niz kierunek ruchu bicia to usuwamy kandydata
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
    std::vector<Move> temp;
    for (auto& i : jumps) {
        makeMove(i);
        bWhiteMove = !bWhiteMove;
        std::vector<Move> nextJumps = findPawnJumps(i.getEndPos(), pawnType);
        for(Move& jump : nextJumps)
            jump.addCaptured(i.getCapturedPositions());
        temp.insert(temp.end(), nextJumps.begin(), nextJumps.end());
        unmakeLastMove();   
        bWhiteMove = !bWhiteMove;
    }
    jumps.insert(jumps.end(), temp.begin(), temp.end());
    for(Move& move : jumps) {
        move = *new Move(pos, move.getEndPos(), pawnType, MoveType::JUMP, MoveDirection::JUMP, move.getCapturedPositions());
    }
    return jumps;
}

std::vector<Move> Board::findLongestJumps(const std::vector<Move>& jumps) {
    std::vector<Move> res;
    int max = 0;
    for(const Move& jump : jumps) {
        if(jump.getCapturedPositions().size() > max) {
            max = jump.getCapturedPositions().size();
            res.clear();
            res.push_back(jump);
        }
        else if(jump.getCapturedPositions().size() == max)
            res.push_back(jump);
    }
    return res;
}
Move Board::findMove(const Move& move) const {
    const auto res = std::find(possibleMoves.begin(), possibleMoves.end(), move);
    if(res == possibleMoves.end())
        throw std::invalid_argument("move");
    return *res;
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
    if(bWhiteMove && (move.getEndPos() > -1 && move.getEndPos() < 4))
        board[move.getEndPos()] = 2;
    else if(!bWhiteMove && (move.getEndPos() > 27 && move.getEndPos() < 32))
        board[move.getEndPos()] = -2;
    else
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

void Board::findInDirection(std::vector<Move>& diagonalsInDirection, const MoveDirection direction, const int startPos, const int pawnType, const bool reversed) const {
    size_t n = diagonalsInDirection.size();
    for(int i = 0; i < n; i++) {
        auto candidates = possibleDiagonals(diagonalsInDirection.at(i).getEndPos(), pawnType, reversed);
        for(const Move& move : candidates) {
            if(move.getMoveDirection() == direction) {
                n++;
                diagonalsInDirection.emplace_back(startPos, move.getEndPos(), pawnType, move.getMoveType(), move.getMoveDirection());
                break;
            }
        }
    }
}

int Board::queenDiagonal(std::vector<Move>& diagonals) const {
    // funkcja zwaraca indeks pierwszego ruchu skierowanego w przeciwna strone
    std::vector<Move> left1, left2, right1, right2;
    for(const auto& move : diagonals) {
        if(move.getEndPos()-move.getStartPos() > 0) {
            if(move.getMoveDirection() == MoveDirection::LEFT)
                left1.push_back(move);
            else if(move.getMoveDirection() == MoveDirection::RIGHT)
                right1.push_back(move);
        }
        else {
            if(move.getMoveDirection() == MoveDirection::LEFT)
                left2.push_back(move);
            else if(move.getMoveDirection() == MoveDirection::RIGHT)
                right2.push_back(move);
        }
    }
    if(!left1.empty())
        findInDirection(left1, MoveDirection::LEFT, left1.at(0).getStartPos(), bWhiteMove ? 2 : -2, left1[0].getStartPos()-left1[0].getEndPos() > 0 ? false : true);
    if(!right1.empty())
        findInDirection(right1, MoveDirection::RIGHT, right1.at(0).getStartPos(), bWhiteMove ? 2 : -2, right1[0].getStartPos()-right1[0].getEndPos() > 0 ? false : true);
    const int res = right1.size() + left1.size();
    
    left1.insert(left1.end(), right1.begin(), right1.end());
    if(!left2.empty())
        findInDirection(left2, MoveDirection::LEFT, left2.at(0).getStartPos(), bWhiteMove ? 2 : -2, left2[0].getStartPos()-left2[0].getEndPos() > 0 ? false : true);
    if(!right2.empty()) {
        findInDirection(right2, MoveDirection::RIGHT, right2.at(0).getStartPos(), bWhiteMove ? 2 : -2, right2[0].getStartPos()-right2[0].getEndPos() > 0 ? false : true);
        left2.insert(left2.end(), right2.begin(), right2.end());
    }
    
    left1.insert(left1.end(), left2.begin(), left2.end());
    diagonals = left1;
    return res;
}

std::vector<Move> Board::possibleDiagonals(const int pos, const int pawnType, const bool reversed) const {
    std::vector<Move> diagonals;
    const int row=pos/4;
    const bool bLastColumn = (pos+1)%4==0 && (pos+1)%8 != 0;
    const bool bFirstColumn = (pos+1)%4==1 && (pos+1)%8 != 1;
    if(row % 2 == 1) {
        if(!bFirstColumn) {
            if(bWhiteMove) {
                if(reversed)
                    diagonals.emplace_back(pos, pos+3, pawnType, MoveType::NORMAL, MoveDirection::LEFT);
                else
                    diagonals.emplace_back(pos, pos-5, pawnType, MoveType::NORMAL, MoveDirection::LEFT);
            }
            else {
                if(reversed)
                    diagonals.emplace_back(pos, pos-5, pawnType, MoveType::NORMAL, MoveDirection::LEFT);
                else
                    diagonals.emplace_back(pos, pos+3, pawnType, MoveType::NORMAL, MoveDirection::LEFT);
            }
        }
        if(bWhiteMove) {
            if(reversed)
                diagonals.emplace_back(pos, pos+4, pawnType, MoveType::NORMAL, MoveDirection::RIGHT);
            else
                diagonals.emplace_back(pos, pos-4, pawnType, MoveType::NORMAL, MoveDirection::RIGHT);
        }
        else {
            if(reversed)
                diagonals.emplace_back(pos, pos-4, pawnType, MoveType::NORMAL, MoveDirection::RIGHT);
            else
                diagonals.emplace_back(pos, pos+4, pawnType, MoveType::NORMAL, MoveDirection::RIGHT);
        }
    }
    else {
        if(!bLastColumn) {
            if(bWhiteMove) {
                if(reversed)
                    diagonals.emplace_back(pos, pos+5, pawnType, MoveType::NORMAL, MoveDirection::RIGHT);
                else
                    diagonals.emplace_back(pos, pos-3, pawnType, MoveType::NORMAL, MoveDirection::RIGHT);
            }
            else {
                if(reversed)
                    diagonals.emplace_back(pos, pos-3, pawnType, MoveType::NORMAL, MoveDirection::RIGHT);
                else
                    diagonals.emplace_back(pos, pos+5, pawnType, MoveType::NORMAL, MoveDirection::RIGHT);
            }
        }
        if(bWhiteMove) {
            if(reversed)
                diagonals.emplace_back(pos, pos+4, pawnType, MoveType::NORMAL, MoveDirection::LEFT);
            else
                diagonals.emplace_back(pos, pos-4, pawnType, MoveType::NORMAL, MoveDirection::LEFT);
        }
        else {
            if(reversed)
                diagonals.emplace_back(pos, pos-4, pawnType, MoveType::NORMAL, MoveDirection::LEFT);
            else
                diagonals.emplace_back(pos, pos+4, pawnType, MoveType::NORMAL, MoveDirection::LEFT);
        }
    }
    for(const auto& move : diagonals) {
        if(move.getEndPos() < 0 || move.getEndPos() > 31) {
            diagonals.erase(std::find(diagonals.begin(), diagonals.end(), move));
        }
    }
    return diagonals;
}

std::vector<Move> Board::findNormalMoves(std::vector<Move> diagonals, bool bIsQueen) const {
    if(bIsQueen) {
        int firstReversed = queenDiagonal(diagonals);
        bool bEndQueenMoveLeft = false, bEndQueenMoveRight = false;
        int startRight = 0;
        for(int i = 0; i < firstReversed; i++) {
            if(diagonals.at(i).getMoveDirection() == MoveDirection::RIGHT) {
                startRight = i;
                break;
            }
        }
        for(int i = 0; i < firstReversed; i++) {
            Move move = diagonals.at(i);
            if(i < startRight) {
                if(bEndQueenMoveLeft) {
                    diagonals.erase(std::find(diagonals.begin(), diagonals.end(), move));
                    i--;
                    startRight--;
                    if(i < firstReversed)
                        firstReversed--;
                    continue;
                }
                if(board[move.getEndPos()] != 0) {
                    bEndQueenMoveLeft = true;
                    diagonals.erase(std::find(diagonals.begin(), diagonals.end(), move));
                    i--;
                    startRight--;
                    if(i < firstReversed)
                        firstReversed--;
                }
            }
            else {
                if(bEndQueenMoveRight) {
                    diagonals.erase(std::find(diagonals.begin(), diagonals.end(), move));
                    i--;
                    if(i < firstReversed)
                        firstReversed--;
                    continue;
                }
                if(board[move.getEndPos()] != 0) {
                    bEndQueenMoveRight = true;
                    diagonals.erase(std::find(diagonals.begin(), diagonals.end(), move));
                    i--;
                    if(i < firstReversed)
                        firstReversed--;
                }
            }
        }
        
        bEndQueenMoveLeft = false;
        bEndQueenMoveRight = false;
        for(int i = firstReversed; i < diagonals.size(); i++) {
            if(diagonals.at(i).getMoveDirection() == MoveDirection::RIGHT) {
                startRight = i;
                break;
            }
        }
        for(int i = firstReversed; i < diagonals.size(); i++) {
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
    return diagonals;
}

std::vector<Move> Board::possibleDiagonalsBoth(int pos, int pawnType) const {
    std::vector<Move> temp1 = possibleDiagonals(pos, pawnType, false);
    std::vector<Move> temp2 = possibleDiagonals(pos, pawnType, true);
    temp1.insert(temp1.end(), temp2.begin(), temp2.end());
    return temp1;
}

bool Board::isLost() const {
    return moves.empty();
}