#pragma once
#include <string>

enum class MoveType
{
    NORMAL, JUMP, NONE
};
enum class MoveDirection
{
    LEFT, RIGHT, JUMP, NONE
};
class Move
{
private:
    int startPos = -1;
    int endPos = -1;
    int pawnType = 0;
    MoveType moveType = MoveType::NONE;
    MoveDirection moveDirection = MoveDirection::NONE;
public:
    Move(int, int, int, MoveType, MoveDirection);
    
    static Move parseMove(const std::string&, int type);
    bool operator== (const Move&) const;
    unsigned getStartPos() const { return startPos; }
    unsigned getEndPos() const { return endPos; }
    int getPawnType() const { return pawnType; }
    MoveType getMoveType() const { return moveType; }
    MoveDirection getMoveDirection() const { return moveDirection; }
};
