#include "Board.h"
#include "Printer.h"
#include <algorithm>
#include <complex>
#include <iostream>
#include <windows.h>

#include "Diagonals.h"
#include "Engine.h"


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
    Diagonals::setBoard(board);
    generateMoves();
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
            if(bIsQueen)
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
            if(!bJumpsPossible) { //jeśli nadal nie znaleziono bic, to szukamy zwyklych ruchow
                std::vector<Move> diagonals;
                if(bIsQueen)
                    diagonals = Diagonals::possibleDiagonalsBoth(i, piece);
                else
                    diagonals = Diagonals::possibleDiagonals(i, piece, false);
                diagonals = findNormalMoves(diagonals, bIsQueen);
                for(const auto& j : diagonals)
                    possibleMoves.push_back(j);
            }
        }
    }
    if(bJumpsPossible) { //jesli znaleziono skoki to szukamy najdluzszych
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

std::vector<Move> Board::findQueenJumps(const int pos, const int pawnType) {
    //sprawdzamy po kolei wszystkie 4 strony w poszukiwaniu bicia
    std::vector<Move> jumps = Diagonals::possibleDiagonalsBoth(pos, pawnType);
    int startReversed = Diagonals::queenDiagonal(jumps), startRight = -1;
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
        //dla każdego bicia wykonujemy je i szukamy następnych
        makeMove(jump, false);
        bWhiteMove = !bWhiteMove;
        Diagonals::changeMove();
        std::vector<Move> nextJumps = findQueenJumps(jump.getEndPos(), pawnType);
        for(Move& m : nextJumps)
            m.addCaptured(jump.getCapturedPositions());
        temp.insert(temp.end(), nextJumps.begin(), nextJumps.end());
        unmakeLastMove(false);   
        bWhiteMove = !bWhiteMove;
        Diagonals::changeMove();
    }
    jumps.insert(jumps.end(), temp.begin(), temp.end());
    for(Move& move : jumps) { //wpisujemy poprawna pozycje startowa dla bicia wielu pionkow
        move = *new Move(pos, move.getEndPos(), pawnType, MoveType::JUMP, MoveDirection::JUMP, move.getCapturedPositions());
    }
    return jumps;
}
std::vector<Move> Board::findPawnJumps(int pos, int pawnType) {
    std::vector<Move> jumps;
    const std::vector<Move> diagonals = Diagonals::possibleDiagonalsBoth(pos, pawnType);
    for(const Move& m : diagonals) {
        if(bWhiteMove ? board[m.getEndPos()] < 0 : board[m.getEndPos()] > 0) { //sprawdza czy bity pionek jest przeciwnego koloru
            std::vector<Move> candidates = Diagonals::possibleDiagonalsBoth(m.getEndPos(), pawnType);
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
    for (Move& i : jumps) {
        //wykonujemy bicie i sprawdzamy kolejne
        makeMove(i, false);
        bWhiteMove = !bWhiteMove;
        std::vector<Move> nextJumps = findPawnJumps(i.getEndPos(), pawnType);
        for(Move& jump : nextJumps)
            jump.addCaptured(i.getCapturedPositions());
        temp.insert(temp.end(), nextJumps.begin(), nextJumps.end());
        unmakeLastMove(false);
        bWhiteMove = !bWhiteMove;
    }
    jumps.insert(jumps.end(), temp.begin(), temp.end());
    for(Move& move : jumps) { //wpisujemy poprawna pozycje startowa dla bicia wielu pionkow
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
void Board::makeMove(const Move& move, const bool bGenerate) {
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
    Diagonals::changeMove();
    Diagonals::setBoard(board);
    const MoveHistory history = {move, capturedPawns};
    moveHistory.push_back(history);
    if(bGenerate)
        generateMoves();
}
void Board::unmakeLastMove(const bool bGenerate) {
    //bGenerate pokazuje, czy nalezy generowac ruchy po wyykonaniu ruchu, poniewaz w przypadku generowania ich niw powinno sie to dziac
    const MoveHistory lastMove = moveHistory.at(moveHistory.size()-1);
    moveHistory.erase(moveHistory.end()-1);
    for(int i = 0 ; i < lastMove.capturedPawns.size(); i++) {
        board[lastMove.move.getCapturedPositions().at(i)] = lastMove.capturedPawns.at(i);
    }
    board[lastMove.move.getStartPos()] = lastMove.move.getPawnType();
    board[lastMove.move.getEndPos()] = 0;
    bWhiteMove = !bWhiteMove;
    Diagonals::changeMove();
    Diagonals::setBoard(board);
    if(bGenerate)
        generateMoves();
}

std::vector<Move> Board::findNormalMoves(std::vector<Move> diagonals, const bool bIsQueen) const {
    if(bIsQueen) {
        int firstReversed = Diagonals::queenDiagonal(diagonals);
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
        int n = diagonals.size();
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