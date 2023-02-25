#include "InputParser.h"

Move InputParser::parseMoveCmd(const std::string& in) {
    std::vector<char> lst {};
    for (int i = 0; i < in.size(); i++) {
        char c = in.at(i);
        if (isspace(c) || c == ',') {
            continue;
        }
        else if (isalnum(c)) {
            lst.push_back(c);
        }
        else {
            break;
        }
    }

    if (lst.size() != 4 
            || !isalpha(lst[0]) || !isalpha(lst[2]) 
            || !isdigit(lst[1]) || ! isdigit(lst[3])
            || lst[1] == '0' || lst[3] == '0'
            || lst[1] == '9' || lst[3] == '9'
            || (lst[0] > 'H' && lst[0] < 'a') || lst[0] > 'h'
            || (lst[2] > 'H' && lst[2] < 'a') || lst[2] > 'h'
        ) 
    {
        throw "Invalid input!";
    }

    int srcX {lst[1]-'0'-1};
    int srcY {lst[0] > 'Z' ? lst[0]-'a' : lst[0]-'A'};
    int dstX {lst[3]-'0'-1};
    int dstY {lst[2] > 'Z' ? lst[2]-'a' : lst[2]-'A'};
    Position src {srcX, srcY};
    Position dst {dstX, dstY};
    return Move {src, dst};
}

PieceType InputParser::parsePromotionCmd(const std::string& in) {
    std::vector<char> lst {};
    for (int i = 0; i < in.size(); i++) {
        char c = in.at(i);
        if (isspace(c)) {
            continue;
        }
        else if (isalpha(c)) {
            lst.push_back(c);
        }
        else {
            break;
        }
    }

    if (lst.size() != 1) {
        throw "Invalid input!";
    }

    char c {lst[0]};
    if (c > 'Z') {
        c -= 32;
    }
    int i = static_cast<int>(c);
    if (i == PAWN) {
        return PAWN;
    }
    else if (i == KNIGHT) {
        return KNIGHT;
    }
    else if (i == BISHOP) {
        return BISHOP;
    }
    else if (i == QUEEN) {
        return QUEEN;
    }
    else if (i == ROOK) {
        return ROOK;
    }
    else {
        throw "Invalid Input!";
    }
}