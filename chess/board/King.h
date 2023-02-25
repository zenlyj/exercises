#pragma once
#include <cstdlib>
#include "Piece.h"

class King : public Piece{
    private:
        bool moved {};
    
    public:
        King(Color color) : Piece(color, KING), moved {false} {}
        void setMoved() {moved = true;}
        bool isMoved() {return moved;}
        bool isCastlingDeviation(const Deviation& deviation) const;
        const std::vector<Deviation> getDeviations() override;
};