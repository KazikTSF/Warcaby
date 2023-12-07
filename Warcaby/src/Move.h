#pragma once
#include <string>

enum MoveType
{
    NORMAL, JUMP, NONE
};
class Move
{
private:
    unsigned startPos = -1;
    unsigned endPos = -1;
    int pawnType = 0;
    MoveType type = NONE;
public:
    Move(int, int, int, MoveType);
    
    static Move parseMove(const std::string&, int type);
    bool operator== (const Move&) const;
    unsigned getStartPos() const { return startPos; }
    unsigned getEndPos() const { return endPos; }
    int getPawnType() const { return pawnType; }
};
