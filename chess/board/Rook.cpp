#include "Rook.h"

const std::vector<Deviation> Rook::getDeviations() {
    return {
        Deviation {0, 1, 7},
        Deviation {0, -1, 7},
        Deviation {1, 0, 7},
        Deviation {-1, 0, 7}
    };
}