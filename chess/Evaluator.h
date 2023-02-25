#pragma once
#include <vector>
#include "board/Board.h"
#include "utils/Move.h"
#include "utils/GameState.h"

class Evaluator {
    private:
        bool isOutOfBounds(const Position& p, const Board& board);
        bool isIllegalPawnMove(const Deviation& deviation, Piece* piece, const Move& move, const Board& board);
        bool isIllegalCastling(const Deviation& deviation, Piece* piece, const Move& move, const Board& board);
        bool isIllegalCheckedMove(Piece* piece, const Move& move, Board board);
        bool isInCheck(const Color& color, const Board& board);
        bool isCheckMate(const Color& color, const Board& board);
        bool isStaleMate(const Board& board);
        std::vector<Move> generateMoves(const Position& src, const Board& board, bool filterIllegalChecked = true, bool filterIllegalCastling = true);

    public:
        bool isLegalMove(const Move& move, const Board& board, const Color& color);
        GameState evaluateBoard(const Board& board);
};