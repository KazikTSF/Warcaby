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
    //test bicia damki
    // board[18] = -2;
    // board[10] = 0;
    // board[3] = 0;
    // board[12] = -1;
    board[13] = 2;
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
            auto jumpsT = findPawnJumps(i, board[i], {});
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
                findNormalMoves(diagonals, bIsQueen);
                for(const auto& j : jumps)
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
std::vector<Move> Board::findQueenJumps(int pos, int pawnType, std::vector<int> captured) {
    std::vector<Move> jumps;
    const std::vector<Move> diagonals = possibleDiagonalsBoth(pos, pawnType);
    int counterCaptured = 0;
    for(const Move& m : diagonals) {
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
    int i = 0;
    for(int i = 0; i < jumps.size(); i++) {;
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
        else if(counterCaptured == 4) {
            if(i == jumps.size()-4) {
                savedCaptured = *savedIterator;
                captured.erase(savedIterator);
                savedIterator = captured.end();
                
            }
            else if(i == jumps.size()-3) {
                savedCaptured = *(savedIterator-1);
                captured.erase(savedIterator-1);
                savedIterator = captured.end()-1;
            }
            else if(i == jumps.size()-2) {
                savedCaptured = *(savedIterator-2);
                captured.erase(savedIterator-2);
                savedIterator = captured.end()-2;
            }
            else if(i == jumps.size()-1) {
                savedCaptured = *(savedIterator-3);
                captured.erase(savedIterator-3);
                savedIterator = captured.end()-3;
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
std::vector<Move> Board::findPawnJumps(int pos, int pawnType, std::vector<int> captured) {
    std::vector<Move> jumps;
    const std::vector<Move> diagonals = possibleDiagonalsBoth(pos, pawnType);
    int counterCaptured = 0;
    for(const Move& m : diagonals) {
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
    int i = 0;
    for(int i = 0; i < jumps.size(); i++) {;
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

void Board::findInDirection(std::vector<Move>& diagonalsInDirection, MoveDirection direction, int startPos, int pawnType) const {//damka sie nie porusza do tylu
    size_t n = diagonalsInDirection.size();
    for(int i = 0; i < n; i++) {
        auto candidates = possibleDiagonals(diagonalsInDirection.at(i).getEndPos(), pawnType, false);
        for(const Move& move : candidates) {
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
    for(const Move& move : diagonals) {
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
        bool bEndQueenMoveLeft = false, bEndQueenMoveRight = false;
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
    for(int i = 0; i < possibleMoves.size(); i++) {
        Move move = possibleMoves.at(i);
        if(i != 0) {
            if(move.getStartPos() != possibleMoves.at(i-1).getStartPos())
                std::cout << std::endl;
        }
        printf("%d - %d;", move.getStartPos()+1, move.getEndPos()+1);
    }
}



