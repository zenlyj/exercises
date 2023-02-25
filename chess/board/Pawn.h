#pragma once
#include <vector>
#include <cstdlib>
#include "Piece.h"

class Pawn : public Piece{
    public:
        Pawn(Color color) : Piece(color, PAWN) {}
        const std::vector<Deviation> getDeviations() override;
        bool isCaptureDeviation(const Deviation& deviation) const;
        bool isDoubleDeviation(const Deviation& deviation) const;
};