#pragma once
#include <ostream>
#include <sstream>
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
    int eval = 0;
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
    int getStartPos() const { return startPos; }
    int getEndPos() const { return endPos; }
    int getPawnType() const { return pawnType; }
    MoveType getMoveType() const { return moveType; }
    MoveDirection getMoveDirection() const { return moveDirection; }
    std::vector<int> getCapturedPositions() const { return capturedPositions; }

    friend bool operator<(const Move& lhs, const Move& rhs) { return lhs.startPos < rhs.startPos; }
    friend bool operator==(const Move& lhs, const Move& rhs) {
        return lhs.startPos == rhs.startPos
            && lhs.endPos == rhs.endPos;
    }
    friend std::ostream& operator<<(std::ostream& os, const Move& obj) {
        std::stringstream ss;
        int start = obj.getStartPos()*2;
        int end = obj.getEndPos()*2;
        const int startRow = 8-start/8;
        const int endRow = 8-end/8;
        if(startRow % 2 == 0)
            start++;
        if(endRow % 2 == 0)
            end++;
        const char startColumn = 'a' + (start % 8);
        const char endColumn = 'a' + (end % 8);
        ss << startColumn << startRow << "-" << endColumn << endRow << "; ";
        return os << ss.str();
    }
};
