#pragma once
#include <string>

enum Color
{
    BLACK, WHITE, NONE
};
class Move
{
private:
    unsigned startPos = -1;
    unsigned endPos = -1;
    int type = 0;
public:
    Move(int, int, int);
    
    static Move parseMove(const std::string&, int type);
    bool operator== (const Move&) const;
    unsigned getStartPos() const { return startPos; }
    unsigned getEndPos() const { return endPos; }
    int getType() const { return type; }
};
