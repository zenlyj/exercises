#pragma once
#include <iostream>
#include <string>
#include "../board/Board.h"

class UserIO {
    public:
        void printBoard(const Board& board);
        void printException(char const* msg);
        void printIllegalMoveMsg();
        void printCheckMsg();
        void printCheckMateMsg();
        void printStaleMateMsg();
        std::string promptInput(const bool& whiteToMove);
        std::string promptPawnPromotion();
};
