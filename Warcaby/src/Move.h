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
    MoveType moveType = MoveType::NONE;
    MoveDirection moveDirection = MoveDirection::NONE;
    std::vector<int> capturedPositions;
public:
    Move(const int start, const int end, const int type, const MoveType mType, const MoveDirection direction) :
        startPos(start), endPos(end), pawnType(type), moveType(mType), moveDirection(direction) {}

    Move(const int start, const int end, const int type, const MoveType mType, const MoveDirection direction,
         const std::vector<int>& captured) : startPos(start), endPos(end), pawnType(type), moveType(mType),
                                             moveDirection(direction), capturedPositions(captured) {}

    void addCaptured(const int pos) { capturedPositions.push_back(pos); }
    void addCaptured(std::vector<int> pos) {
        capturedPositions.insert(capturedPositions.end(), pos.begin(), pos.end());
    }
    static Move parseMove(const std::string&, int type);
    bool operator== (const Move&) const;
    int getStartPos() const { return startPos; }
    int getEndPos() const { return endPos; }
    int getPawnType() const { return pawnType; }
    MoveType getMoveType() const { return moveType; }
    MoveDirection getMoveDirection() const { return moveDirection; }
    std::vector<int> getCapturedPositions() const { return capturedPositions; }
};
