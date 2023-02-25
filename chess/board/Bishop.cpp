#include "Bishop.h"

const std::vector<Deviation> Bishop::getDeviations() {
    return {
        Deviation {1, 1, 7},
        Deviation {-1, -1, 7},
        Deviation {1, -1, 7},
        Deviation {-1, 1, 7}
    };
}