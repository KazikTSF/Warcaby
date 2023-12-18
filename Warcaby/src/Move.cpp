#include "Move.h"

#include <sstream>
#include <stdexcept>

Move Move::parseMove(const std::string& move, int type) {
    int start, end;
    if(isalpha(move[0])) {
        if(move.size() != 5)
            throw std::invalid_argument("move.size()");
        start = move[0]-'a' + 8*(8-(move[1]-'0'));
        end = move[3]-'a' + 8*(8-(move[4]-'0'));
        bool evenRowStart = start/8 % 2 == 0;
        bool evenRowEnd = end/8 % 2 == 0;
        if((start % 2 == 1 && !evenRowStart) || (end %2 == 1 && !evenRowEnd))
            throw std::invalid_argument("move");
        if((start % 2 == 0 && evenRowStart) || (end %2 == 0 && evenRowEnd))
            throw std::invalid_argument("move");
        start /= 2;
        end /= 2;
    } else {
        std::stringstream ss(move);
        std::string temp;
        std::getline(ss, temp, '-');
        start = std::stoi(temp)-1;
        std::getline(ss, temp, '-');
        end = std::stoi(temp)-1;
    }
    if(abs(start-end)>5)
        return {start, end, type, MoveType::JUMP, MoveDirection::JUMP};
    return {start, end, type, MoveType::NORMAL, MoveDirection::NONE};
}
