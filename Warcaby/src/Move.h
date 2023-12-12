#pragma once
#include <string>
#include <vector>

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
    std::vector<int> capturedPositions;
    MoveType moveType = MoveType::NONE;
    MoveDirection moveDirection = MoveDirection::NONE;
public:
    Move(int start, int end, int type, MoveType mType, MoveDirection direction);
    Move(int start, int end, int type, MoveType mType, MoveDirection direction, int capturedPos);
    static Move parseMove(const std::string&, int type);
    bool operator== (const Move&) const;
    int getStartPos() const { return startPos; }
    int getEndPos() const { return endPos; }
    int getPawnType() const { return pawnType; }
    MoveType getMoveType() const { return moveType; }
    MoveDirection getMoveDirection() const { return moveDirection;}
    std::vector<int> getCapturedPositions() const { return capturedPositions; }
};
