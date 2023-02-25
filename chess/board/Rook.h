#pragma once
#include "Piece.h"

class Rook : public Piece{
    private:
        bool moved {};

    public:
        Rook(Color color) : Piece(color, ROOK), moved {false} {}
        void setMoved() {moved = true;}
        bool isMoved() {return moved;}
        const std::vector<Deviation> getDeviations() override;
};