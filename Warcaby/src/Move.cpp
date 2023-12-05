#include "Move.h"

#include <stdexcept>

Move::Move(const int start, const int end, const int type) : startPos(start), endPos(end), type(type) {}
Move Move::parseMove(const std::string& move, int type) {
    int start, end;
    if(move.size() != 5)
        throw std::invalid_argument("move.size()");
    if(isalpha(move[0])) {
        start = (move[0] - 'a') + (8 - (move[1] - '0')) * 4;
        end = (move[3] - 'a') + (8 - (move[4] - '0')) * 4;
    } else {
        start = (move[0] - '0') + (8 - (move[1] - '0')) * 4;
        end = (move[3] - '0') + (8 - (move[4] - '0')) * 4;
    }
    return {start, end, type};
}

bool Move::operator== (const Move& m) const{
    return startPos == m.startPos && endPos == m.endPos && type == m.type;
}
