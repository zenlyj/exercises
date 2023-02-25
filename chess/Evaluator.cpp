#include "Evaluator.h"

bool Evaluator::isOutOfBounds(const Position& p, const Board& board) {
    int x {p.getX()};
    int y {p.getY()};
    if (x < 0 || x >= board.getNumRows()) {
        return true;
    }
    else if (y < 0 || y >= board.getNumCols()) {
        return true;
    }
    else {
        return false;
    }
}

bool Evaluator::isIllegalPawnMove(const Deviation& deviation, Piece* piece, const Move& move, const Board& board) {
    const Position& src {move.getSrc()};
    const Position& dst {move.getDst()};
    Pawn* pawn = dynamic_cast<Pawn*>(piece);
    if (board.getPiece(dst) && !pawn->isCaptureDeviation(deviation)) {
        return true;
    }
    if (pawn->isCaptureDeviation(deviation) && !board.getPiece(dst)) {
        return true;
    }
    if (pawn->isDoubleDeviation(deviation)) {
        int x {src.getX()};
        if (pawn->getColor() == WHITE && x != 1) return true;
        if (pawn->getColor() == BLACK && x != 6) return true;
    }
    return false;
}

bool Evaluator::isIllegalCastling(const Deviation& deviation, Piece* piece, const Move& move, const Board& board) {
    if (isInCheck(piece->getColor(), board)) return true;
    King* king = dynamic_cast<King*>(piece);
    if (king->isMoved()) return true;
    Position src {move.getSrc()};
    Position dst {move.getDst()};
    int srcX {src.getX()};
    int srcY {src.getY()};
    int dstX {dst.getX()};
    int dstY {dst.getY()};
    Piece* tempRook {nullptr};
    if (srcY < dstY) {
        for (int i = srcY+1; i < board.getNumCols()-1; i++) {
            Position betweenPos {srcX, i};
            if (board.getPiece(betweenPos)) return true;
        }
        for (int i = srcY+1; i <= dstY; i++) {
            Position kingPath {srcX, i};
            Move kingMove {src, kingPath};
            if (isIllegalCheckedMove(piece, kingMove, board)) return true;
        }
        Position rookPos {srcX, board.getNumCols()-1};
        tempRook = board.getPiece(rookPos);
    }
    else {
        for (int i = srcY-1; i > 0; i--) {
            Position betweenPos {srcX, i};
            if (board.getPiece(betweenPos)) return true;
        }
        for (int i = srcY-1; i >= dstY; i--) {
            Position kingPath {srcX, i};
            Move kingMove {src, kingPath};
            if (isIllegalCheckedMove(piece, kingMove, board)) return true;
        }
        Position rookPos {srcX, 0};
        tempRook = board.getPiece(rookPos);
    }

    if (!tempRook) return true;
    if (tempRook->getType() != ROOK) return true;
    if (tempRook->getColor() != king->getColor()) return true;
    Rook* rook = dynamic_cast<Rook*>(tempRook);
    if (rook->isMoved()) return true;
    return false;
}

bool Evaluator::isIllegalCheckedMove(Piece* piece, const Move& move, Board board) {
    board.movePiece(move);
    return isInCheck(piece->getColor(), board);
}

bool Evaluator::isStaleMate(const Board& board) {
    std::vector<std::vector<Move>> whiteMoves {};
    std::vector<std::vector<Move>> blackMoves {};
    for (int i = 0; i < board.getNumRows(); i++) {
        for (int j = 0; j < board.getNumCols(); j++) {
            Position pos {i, j};
            Piece* piece {board.getPiece(pos)};
            if (piece && piece->getColor() == WHITE) {
                whiteMoves.push_back(generateMoves(pos, board));
            }
            else {
                blackMoves.push_back(generateMoves(pos, board));
            }
        }
    }
    bool res {true};
    for (const auto& pieceMoves : whiteMoves) {
        res &= pieceMoves.empty();
    }
    if (res) return true;
    res = true;
    for (const auto& pieceMoves : blackMoves) {
        res &= pieceMoves.empty();
    }
    return res;
}

bool Evaluator::isInCheck(const Color& color, const Board& board) {
    std::vector<std::vector<Move>> moves {};
    for (int i = 0; i < board.getNumRows(); i++) {
        for (int j = 0; j < board.getNumCols(); j++) {
            Position pos {i, j};
            Piece* piece {board.getPiece(pos)};
            if (piece && piece->getColor() != color) {
                moves.push_back(generateMoves(pos, board, false, false));
            }
        }
    }
    for (auto& pieceMoves : moves) {
        for (auto& move : pieceMoves) {
            Position dst {move.getDst()};
            Piece* piece {board.getPiece(dst)};
            if (piece && piece->getType() == KING) {
                return true;
            }
        }
    }
    return false;
}

bool Evaluator::isCheckMate(const Color& color, const Board& board) {
    std::vector<std::vector<Move>> moves {};
    for (int i = 0; i < board.getNumRows(); i++) {
        for (int j = 0; j < board.getNumCols(); j++) {
            Position pos {i, j};
            Piece* piece {board.getPiece(pos)};
            if (piece && piece->getColor() == color) {
                moves.push_back(generateMoves(pos, board));
            }
        }
    }
    for (const auto& pieceMoves : moves) {
        for (const auto& move : pieceMoves) {
            Board copy = board;
            copy.movePiece(move);
            if (!isInCheck(color, copy)) {
                return false;
            }
        }
    }
    return true;
}

std::vector<Move> Evaluator::generateMoves(const Position& src, const Board& board, bool filterIllegalChecked, bool filterIllegalCastling) {
    std::vector<Move> res {};
    Piece* srcPiece {board.getPiece(src)};
    if (!srcPiece) return res;
    for (const auto& deviation : srcPiece->getDeviations()) {
        for (int i = 1; i <= deviation.getFreq(); i++) {
            int x {src.getX() + deviation.getX() * i};
            int y {src.getY() + deviation.getY() * i};
            Position dst {x, y};
            if (isOutOfBounds(dst, board)) {
                break;
            }
            Piece* dstPiece {board.getPiece(dst)};
            Move genMove {src, dst};
            if (dstPiece && dstPiece->getColor() == srcPiece->getColor()) {
                break;
            }
            if (srcPiece->getType() == PAWN && isIllegalPawnMove(deviation, srcPiece, genMove, board)) {
                break;
            }
            if (filterIllegalChecked && isIllegalCheckedMove(srcPiece, genMove, board)) {
                continue;
            }
            if (srcPiece->getType() == KING && (dynamic_cast<King*>(srcPiece))->isCastlingDeviation(deviation)) {
                if (!filterIllegalCastling) continue;
                if (isIllegalCastling(deviation, srcPiece, genMove, board)) continue;
            }
            res.push_back(genMove);
            if (dstPiece) {
                break;
            }
        }
    }
    return res;
}

bool Evaluator::isLegalMove(const Move& move, const Board& board, const Color& color) {
    Position src {move.getSrc()};
    if (board.getPiece(src) && board.getPiece(src)->getColor() != color) return false;
    std::vector<Move> possibleMoves {generateMoves(src, board)};
    for (auto& m : possibleMoves) {
        if (move.isEqual(m)) {
            return true;
        }
    }
    return false;
}

GameState Evaluator::evaluateBoard(const Board& board) {
    GameState state {IN_PROGRESS};
    bool whiteAttacked {isInCheck(WHITE, board)};
    bool blackAttacked {isInCheck(BLACK, board)};
    
    state = whiteAttacked || blackAttacked ? IN_CHECK : state;

    if (whiteAttacked && isCheckMate(WHITE, board)) {
        state = CHECK_MATE;
    }
    else if (blackAttacked && isCheckMate(BLACK, board)) {
        state = CHECK_MATE;
    }
    else if (isStaleMate(board)) {
        state = STALE_MATE;
    }

    return state;
}