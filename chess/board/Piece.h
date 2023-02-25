#pragma once
#include "../utils/PieceType.h"
#include "../utils/Color.h"
#include "../utils/Deviation.h"
#include <vector>

class Piece {
    protected:
        PieceType mType {};
        Color mColor {};
    public:
        Piece(Color color, PieceType type) : mColor {color}, mType {type} {}
        const PieceType& getType() {return mType;}
        const Color& getColor() {return mColor;}
        virtual const std::vector<Deviation> getDeviations() = 0;
};