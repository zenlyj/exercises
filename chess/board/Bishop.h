#pragma once
#include "Piece.h"

class Bishop : public Piece {
    public:
        Bishop(Color color) : Piece(color, BISHOP) {};
        const std::vector<Deviation> getDeviations() override;
};