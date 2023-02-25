#include "Queen.h"

const std::vector<Deviation> Queen::getDeviations() {
    return {
        Deviation {0, 1, 7},
        Deviation {0, -1, 7},
        Deviation {1, 0, 7},
        Deviation {-1, 0, 7},
        Deviation {1, 1, 7},
        Deviation {-1, -1, 7},
        Deviation {1, -1, 7},
        Deviation {-1, 1, 7}
    };
}