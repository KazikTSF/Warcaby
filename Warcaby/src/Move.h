#pragma once
enum Color
{
    BLACK, WHITE, NONE
};
class Move
{
private:
    unsigned startPos = -1;
    unsigned endPos = -1;
    Color color = NONE;
public:
    Move(int, int, Color);
    unsigned getStartPos() const { return startPos; }
    unsigned getEndPos() const { return endPos; }
    Color getColor() const { return color; }
};
