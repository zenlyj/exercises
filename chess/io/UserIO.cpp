#include "UserIO.h"

void UserIO::printBoard(const Board& board) {
    std::cout << "\n\n" << board.toString() << "\n\n";
}

void UserIO::printException(char const* msg) {
    std::cout << '\n' << msg << '\n';
}

void UserIO::printIllegalMoveMsg() {
    std::cout << '\n' << "Illegal Move!" << '\n';
}

void UserIO::printCheckMsg() {
    std::cout << '\n' << "Check!" << '\n';
}

void UserIO::printCheckMateMsg() {
    std::cout << '\n' << "Check Mate!" << '\n';
}

void UserIO::printStaleMateMsg() {
    std::cout << '\n' << "Stale Mate!" << '\n';
}

std::string UserIO::promptInput(const bool& whiteToMove) {
    std::cout << '\n' << (whiteToMove ? "White" : "Black") << " to move. eg. B1, C3" << '\n';
    std::string input {};
    std::getline(std::cin >> std::ws, input);
    return input;
}

std::string UserIO::promptPawnPromotion() {
    std::cout << '\n' << "Pawn promoted to: eg. Q = Queen" << '\n';
    std::string input {};
    std::getline(std::cin >> std::ws, input);
    return input;
}