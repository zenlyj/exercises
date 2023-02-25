#pragma once
#include <string>
#include <vector>
#include "../utils/Move.h"
#include "../utils/PieceType.h"

class InputParser {
    public:
        Move parseMoveCmd(const std::string& in);
        PieceType parsePromotionCmd(const std::string& in);
};
