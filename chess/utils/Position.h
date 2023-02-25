#pragma once

class Position {
    private:
        const int mX {};
        const int mY {};
    
    public:
        Position(int x = 0, int y = 0) : mX {x}, mY {y} {}
        const int getX() const {return mX;}
        const int getY() const {return mY;}
        bool isEqual(const Position& p) const {return mX == p.getX() && mY == p.getY();}
};