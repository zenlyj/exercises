#pragma once
#include "Piece.h"

class Knight : public Piece {
    public:
        Knight(Color color) : Piece(color, KNIGHT) {}
        const std::vector<Deviation> getDeviations() override;
};