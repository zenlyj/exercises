#include "Pawn.h"

const std::vector<Deviation> Pawn::getDeviations() {
    int advanceDirection {mColor == WHITE ? 1 : -1};
    return {
        Deviation {advanceDirection, 0},
        Deviation {advanceDirection*2, 0},
        Deviation {advanceDirection, advanceDirection},
        Deviation {advanceDirection, 0-advanceDirection}
    };
}

bool Pawn::isCaptureDeviation(const Deviation& deviation) const {
    return abs(deviation.getX()) == abs(deviation.getY());
}

bool Pawn::isDoubleDeviation(const Deviation& deviation) const {
    return abs(deviation.getX()) > 1;
}