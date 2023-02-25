#pragma once
#include "Position.h"

class Move {
    private:
        const Position mSrc {};
        const Position mDst {};
    
    public:
        Move(Position src, Position dst) : mSrc {src}, mDst {dst} {}
        const Position& getSrc() const {return mSrc;}
        const Position& getDst() const {return mDst;}
        bool isEqual(const Move& m) const {return mSrc.isEqual(m.getSrc()) && mDst.isEqual(m.getDst());}
};