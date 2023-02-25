#pragma once

class Deviation {
    private:
        const int mX {};
        const int mY {};
        const int mFreq {};
    
    public:
        Deviation(int x, int y, int freq=1) : mX {x}, mY {y}, mFreq {freq} {}
        const int getX() const {return mX;}
        const int getY() const {return mY;}
        const int getFreq() const {return mFreq;} 
};