#include "Knight.h"

const std::vector<Deviation> Knight::getDeviations() {
    return {
        Deviation {2, -1},
        Deviation {2, 1},
        Deviation {-2, -1},
        Deviation {-2, 1},
        Deviation {-1, 2},
        Deviation {-1, -2},
        Deviation {1, 2},
        Deviation {1, -2}
    };
}