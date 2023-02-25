#include "King.h"

const std::vector<Deviation> King::getDeviations() {
    return {
        Deviation {0, 1},
        Deviation {0, -1},
        Deviation {1, 0},
        Deviation {-1, 0},
        Deviation {1, 1},
        Deviation {1, -1},
        Deviation {-1, 1},
        Deviation {-1, -1},
        Deviation {0, -2},
        Deviation {0, 2}
    };
}

bool King::isCastlingDeviation(const Deviation& deviation) const {
    return abs(deviation.getY()) > 1;
}