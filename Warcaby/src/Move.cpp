#include "Move.h"

#include <stdexcept>

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
    if(abs(start-end)>5)
        return {start, end, type, MoveType::JUMP, MoveDirection::JUMP};
    auto direction = MoveDirection::NONE;
    const bool row = start/4 % 2 == 1; 
    switch (start-end) {
    case 3:
    case -5:
        direction = MoveDirection::LEFT;
        break;
    case -3:
    case 5:
        direction = MoveDirection::RIGHT;
        break;
    
    case 4:
    case -4:
        if(row)
            direction = MoveDirection::RIGHT;
        else
            direction = MoveDirection::LEFT;
        break;
    default:
          throw std::invalid_argument("move");
                
    }
    return {start, end, type, MoveType::NORMAL, direction};
}
