#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include "Piece.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "King.h"
#include "Queen.h"
#include "../utils/Position.h"
#include "../utils/Move.h"

constexpr int numRows {8};
constexpr int numCols {8};
const std::vector<std::vector<int>> initialPawnPos {
    {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1,6}, {1, 7}
};
const std::vector<std::vector<int>> initialKnightPos {
    {0, 1}, {0, numCols-2}
};
const std::vector<std::vector<int>> initialBishopPos {
    {0, 2}, {0, numCols-3}
};
const std::vector<std::vector<int>> initialRookPos {
    {0, 0}, {0, numCols-1}
};
const std::vector<std::vector<int>> initialKingPos {
    {0, 3}
};
const std::vector<std::vector<int>> initialQueenPos {
    {0, 4}
};

class Board {
    private:
        std::vector<std::vector<Piece*>> mState {};
        std::unordered_set<Piece*> captured {};

    public:
        Board();
        void destroy();
        const std::string toString() const;
        Piece* getPiece(const Position& pos) const;
        void setPiece(const Position& pos, Piece* piece);
        void movePiece(const Move& move);
        int getNumRows() const;
        int getNumCols() const;
};