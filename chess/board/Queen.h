#pragma once
#include "Piece.h"

class Queen : public Piece{
    public:
        Queen(Color color) : Piece(color, QUEEN) {}
        const std::vector<Deviation> getDeviations() override;
};