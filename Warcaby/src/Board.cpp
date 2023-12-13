#include "Board.h"

#include <algorithm>
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
    board[23] = 2;
    board[18] = -1;
    board[10] = 0;
    board[3] = 0;
    board[12] = -1;
    
    printBoard();
}
bool Board::isMoveLegal(const Move& move) const {
    if(std::any_of(possibleMoves.begin(), possibleMoves.end(), [&move](const Move& m) {return m == move;}))
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
            std::vector<Move> jumpsT;
            if(abs(piece) == 2)
                jumpsT = findQueenJumps(i, board[i], {});
            else 
                jumpsT = findPawnJumps(i, board[i], {});
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
                std::vector<Move> diagonals = possibleDiagonals(i, piece, false);
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
std::vector<Move> Board::findQueenJumps(int pos, int pawnType, std::vector<int> captured) { //kto to pisal, czas na lekcje DRY
    std::vector<Move> jumps = possibleDiagonalsBoth(pos, pawnType);
    int startReversed = queenDiagonal(jumps), startRight = -1, counterCaptured = 0, capturedPosInOneDiagonal = -1;
    for(int i = 0; i < startReversed; i++) {
        if(jumps[i].getMoveDirection() == MoveDirection::RIGHT) {
            startRight = i;
            break;
        }
    }
    bool endDiagonal = false;
    for(int i = 0; i < startRight; i++) {
        const Move move = jumps.at(i);
        if(capturedPosInOneDiagonal != -1)
            jumps.at(i).addCaptured(capturedPosInOneDiagonal);
        if(endDiagonal) {
            jumps.erase(std::find(jumps.begin(), jumps.end(), move));
            i--;
            startRight--;
            startReversed--;
            continue;
        }
        if(bWhiteMove ? board[move.getEndPos()] < 0 : board[move.getEndPos()] > 0) {
            capturedPosInOneDiagonal = move.getEndPos();
            endDiagonal = true;
            counterCaptured++;
            captured.push_back(move.getEndPos());
            jumps.erase(std::find(jumps.begin(), jumps.end(), move));
        }
        else if(bWhiteMove ? board[move.getEndPos()] > 0 : board[move.getEndPos()] < 0) {
            endDiagonal = true;
            jumps.erase(std::find(jumps.begin(), jumps.end(), move));
            startReversed--;
        }
    }
    endDiagonal = false;
    capturedPosInOneDiagonal = -1;
    for(int i = startRight; i < startReversed; i++) {
        const Move move = jumps.at(i);
        if(capturedPosInOneDiagonal != -1)
            jumps.at(i).addCaptured(capturedPosInOneDiagonal);
        if(endDiagonal) {
            jumps.erase(std::find(jumps.begin(), jumps.end(), move));
            i--;
            startRight--;
            startReversed--;
            continue;
        }
        if(bWhiteMove ? board[move.getEndPos()] < 0 : board[move.getEndPos()] > 0) {
            capturedPosInOneDiagonal = move.getEndPos();
            endDiagonal = true;
            counterCaptured++;
            captured.push_back(move.getEndPos());
            jumps.erase(std::find(jumps.begin(), jumps.end(), move));
            startReversed--;
        }
        else if(bWhiteMove ? board[move.getEndPos()] > 0 : board[move.getEndPos()] < 0) {
            endDiagonal = true;
            jumps.erase(std::find(jumps.begin(), jumps.end(), move));
            startReversed--;
        }
    }
    for(int i = startReversed; i < jumps.size(); i++) {
        if(jumps[i].getMoveDirection() == MoveDirection::RIGHT) {
            startRight = i;
            break;
        }
    }
    endDiagonal = false;
    capturedPosInOneDiagonal = -1;
    
    for(int i = startReversed; i < startRight; i++) {
        const Move move = jumps.at(i);
        if(capturedPosInOneDiagonal != -1)
            jumps.at(i).addCaptured(capturedPosInOneDiagonal);
        if(endDiagonal) {
            jumps.erase(std::find(jumps.begin(), jumps.end(), move));
            i--;
            startRight--;
            continue;
        }
        if(bWhiteMove ? board[move.getEndPos()] < 0 : board[move.getEndPos()] > 0) {
            capturedPosInOneDiagonal = move.getEndPos();
            endDiagonal = true;
            counterCaptured++;
            captured.push_back(move.getEndPos());
            jumps.erase(std::find(jumps.begin(), jumps.end(), move));
            startReversed--;
            startRight--;
        }
        else if(bWhiteMove ? board[move.getEndPos()] > 0 : board[move.getEndPos()] < 0) {
            endDiagonal = true;
            jumps.erase(std::find(jumps.begin(), jumps.end(), move));
            startRight--;
        }
    }
    endDiagonal = false;
    capturedPosInOneDiagonal = -1;
    for(int i = startRight; i < jumps.size(); i++) {
        const Move move = jumps.at(i);
        if(capturedPosInOneDiagonal != -1)
            jumps.at(i).addCaptured(capturedPosInOneDiagonal);
        if(endDiagonal) {
            jumps.erase(std::find(jumps.begin(), jumps.end(), move));
            i--;
            continue;
        }
        if(bWhiteMove ? board[move.getEndPos()] < 0 : board[move.getEndPos()] > 0) {
            capturedPosInOneDiagonal = move.getEndPos();
            endDiagonal = true;
            counterCaptured++;
            captured.push_back(move.getEndPos());
            jumps.erase(std::find(jumps.begin(), jumps.end(), move));
        }
        else if(bWhiteMove ? board[move.getEndPos()] > 0 : board[move.getEndPos()] < 0) {
            endDiagonal = true;
            jumps.erase(std::find(jumps.begin(), jumps.end(), move));
        }
    }
    if(jumps.empty()) //TODO znaleziona diagonala, kontynuacja
        return {};
    std::vector<Move> temp;
    for(int i = 0; i < jumps.size(); i++) {
        makeMove(jumps.at(i));
        bWhiteMove = !bWhiteMove;
        int savedCaptured;
        std::vector<int>::const_iterator savedIterator = captured.end()-1;
        if(counterCaptured == 2) {
            if(i == jumps.size()-2) {
                savedCaptured = *savedIterator;
                captured.erase(savedIterator);
                savedIterator = captured.end();
            }
            else if(i == jumps.size()-1) {
                savedCaptured = *(savedIterator-1);
                captured.erase(savedIterator-1);
                savedIterator = captured.end()-1;
            }
        }
        else if(counterCaptured == 3) {
            if(i == jumps.size()-3) {
                savedCaptured = *savedIterator;
                captured.erase(savedIterator);
                savedIterator = captured.end();
                
            }
            else if(i == jumps.size()-2) {
                savedCaptured = *(savedIterator-1);
                captured.erase(savedIterator-1);
                savedIterator = captured.end()-1;
            }
            else if(i == jumps.size()-1) {
                savedCaptured = *(savedIterator-2);
                captured.erase(savedIterator-2);
                savedIterator = captured.end()-2;
            }
        }
        jumps.at(i) = *new Move(jumps.at(i).getEndPos(), jumps.at(i).getEndPos(), pawnType, MoveType::JUMP, MoveDirection::JUMP, captured);
        std::vector<Move> nextJumps = findQueenJumps(jumps.at(i).getEndPos(), pawnType, captured);//TODO error on thrid
        temp.insert(temp.end(), nextJumps.begin(), nextJumps.end());
        unmakeLastMove();   
        bWhiteMove = !bWhiteMove;
        if(counterCaptured > 1) {
            captured.insert(savedIterator, savedCaptured);
        }
    }
    jumps.insert(jumps.end(), temp.begin(), temp.end());
    for(Move& move : jumps) {
        move = *new Move(pos, move.getEndPos(), pawnType, MoveType::JUMP, MoveDirection::JUMP, move.getCapturedPositions());
    }
    return jumps;
}
std::vector<Move> Board::findPawnJumps(int pos, int pawnType, std::vector<int> captured) {
    std::vector<Move> jumps;
    const std::vector<Move> diagonals = possibleDiagonalsBoth(pos, pawnType);
    int counterCaptured = 0;
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
                jumps.at(jumps.size()-1).addCaptured(captured);
                jumps.at(jumps.size()-1).addCaptured(m.getEndPos());
                auto temp = jumps.at(jumps.size()-1).getCapturedPositions();
                for(const int i : temp) {
                    if(std::find(captured.begin(), captured.end(), i) == captured.end())
                        captured.push_back(i);
                }
                counterCaptured++;
            }
        }
    }
    if(jumps.empty())
        return {};
    std::vector<Move> temp;
    for(int i = 0; i < jumps.size(); i++) {
        makeMove(jumps.at(i));
        bWhiteMove = !bWhiteMove;
        int savedCaptured;
        std::vector<int>::const_iterator savedIterator = captured.end()-1;
        if(counterCaptured == 2) {
            if(i == jumps.size()-2) {
                savedCaptured = *savedIterator;
                captured.erase(savedIterator);
                savedIterator = captured.end();
            }
            else if(i == jumps.size()-1) {
                savedCaptured = *(savedIterator-1);
                captured.erase(savedIterator-1);
                savedIterator = captured.end()-1;
            }
        }
        else if(counterCaptured == 3) {
            if(i == jumps.size()-3) {
                savedCaptured = *savedIterator;
                captured.erase(savedIterator);
                savedIterator = captured.end();
                
            }
            else if(i == jumps.size()-2) {
                savedCaptured = *(savedIterator-1);
                captured.erase(savedIterator-1);
                savedIterator = captured.end()-1;
            }
            else if(i == jumps.size()-1) {
                savedCaptured = *(savedIterator-2);
                captured.erase(savedIterator-2);
                savedIterator = captured.end()-2;
            }
        }
        jumps.at(i) = *new Move(jumps.at(i).getEndPos(), jumps.at(i).getEndPos(), pawnType, MoveType::JUMP, MoveDirection::JUMP, captured);
        std::vector<Move> nextJumps = findPawnJumps(jumps.at(i).getEndPos(), pawnType, captured);
        temp.insert(temp.end(), nextJumps.begin(), nextJumps.end());
        unmakeLastMove();   
        bWhiteMove = !bWhiteMove;
        if(counterCaptured > 1) {
            captured.insert(savedIterator, savedCaptured);
        }
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

void Board::findInDirection(std::vector<Move>& diagonalsInDirection, MoveDirection direction, int startPos, int pawnType, bool reversed) const {
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
    int i = 0;
    for(; i < 2; i++) {
        const auto& move = diagonals[i];
        if(move.getMoveDirection() == MoveDirection::LEFT)
            left1.push_back(move);
        else if(move.getMoveDirection() == MoveDirection::RIGHT)
            right1.push_back(move);
    }
    findInDirection(left1, MoveDirection::LEFT, left1.at(0).getStartPos(), bWhiteMove ? 2 : -2, false);
    findInDirection(right1, MoveDirection::RIGHT, right1.at(0).getStartPos(), bWhiteMove ? 2 : -2, false);
    const int res = right1.size() + left1.size();
    
    left1.insert(left1.end(), right1.begin(), right1.end());
    for(; i < 4; i++) {
        const auto& move = diagonals[i];
        if(move.getMoveDirection() == MoveDirection::LEFT)
            left2.push_back(move);
        else if(move.getMoveDirection() == MoveDirection::RIGHT)
            right2.push_back(move);
    }
    findInDirection(left2, MoveDirection::LEFT, left2.at(0).getStartPos(), bWhiteMove ? 2 : -2, true);
    findInDirection(right2, MoveDirection::RIGHT, right2.at(0).getStartPos(), bWhiteMove ? 2 : -2, true);
    left2.insert(left2.end(), right2.begin(), right2.end());
    
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
    for(const auto& move : diagonals) {
        if(move.getEndPos() < 0 || move.getEndPos() > 31) {
            diagonals.erase(std::find(diagonals.begin(), diagonals.end(), move));
        }
    }
    return diagonals;
}

std::vector<Move> Board::findNormalMoves(std::vector<Move> diagonals, bool bIsQueen) const {
    if(bIsQueen) {
        diagonals = possibleDiagonalsBoth(diagonals.at(0).getStartPos(), diagonals.at(0).getPawnType());
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
    for(int i = 0; i < possibleMoves.size(); i++) {
        Move move = possibleMoves.at(i);
        if(i != 0) {
            if(move.getStartPos() != possibleMoves.at(i-1).getStartPos())
                std::cout << std::endl;
        }
        printf("%d - %d; ", move.getStartPos()+1, move.getEndPos()+1);
    }
}



