#include "Diagonals.h"

#include <algorithm>
int Diagonals::board[32] = {};
bool Diagonals::bWhiteMove = true;
void Diagonals::setBoard(int boardInit[32]) {
    std::copy_n(boardInit, 32, board);
}

void Diagonals::changeMove() {
    bWhiteMove = !bWhiteMove;
}

int Diagonals::queenDiagonal(std::vector<Move>& diagonals) {
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
        findInDirection(left1, MoveDirection::LEFT, left1.at(0).getStartPos(), bWhiteMove ? 2 : -2, left1[0].getStartPos()-left1[0].getEndPos() > 0 ? !bWhiteMove : bWhiteMove);
    if(!right1.empty())
        findInDirection(right1, MoveDirection::RIGHT, right1.at(0).getStartPos(), bWhiteMove ? 2 : -2, right1[0].getStartPos()-right1[0].getEndPos() > 0 ? !bWhiteMove : bWhiteMove);
    const int res = right1.size() + left1.size();
    
    left1.insert(left1.end(), right1.begin(), right1.end());
    if(!left2.empty())
           findInDirection(left2, MoveDirection::LEFT, left2.at(0).getStartPos(), bWhiteMove ? 2 : -2, left2[0].getStartPos()-left2[0].getEndPos() > 0 ? !bWhiteMove : bWhiteMove);
 if(!right2.empty()) {
        findInDirection(right2, MoveDirection::RIGHT, right2.at(0).getStartPos(), bWhiteMove ? 2 : -2, right2[0].getStartPos()-right2[0].getEndPos() > 0 ? !bWhiteMove : bWhiteMove);
        left2.insert(left2.end(), right2.begin(), right2.end());
    }
    
    left1.insert(left1.end(), left2.begin(), left2.end());
    diagonals = left1;
    return res;
}

std::vector<Move> Diagonals::possibleDiagonals(const int pos, const int pawnType, const bool reversed) {
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
void Diagonals::findInDirection(std::vector<Move>& diagonalsInDirection, const MoveDirection direction, const int startPos, const int pawnType, const bool reversed) {
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
std::vector<Move> Diagonals::possibleDiagonalsBoth(int pos, int pawnType) {
    std::vector<Move> temp1 = Diagonals::possibleDiagonals(pos, pawnType, false);
    std::vector<Move> temp2 = Diagonals::possibleDiagonals(pos, pawnType, true);
    temp1.insert(temp1.end(), temp2.begin(), temp2.end());
    return temp1;
}