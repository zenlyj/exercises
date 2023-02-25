#include "board/Board.h"
#include "Evaluator.h"
#include "io/UserIO.h"
#include "io/InputParser.h"

void setMoved(Move& move, const Board& board) {
    Position src {move.getSrc()};
    Piece* srcPiece {board.getPiece(src)};
    if (!srcPiece) return;
    if (srcPiece->getType() == KING) {
        King* king = dynamic_cast<King*>(srcPiece);
        king->setMoved();
    }
    else if (srcPiece->getType() == ROOK) {
        Rook* rook = dynamic_cast<Rook*>(srcPiece);
        rook->setMoved();
    }
}

void castle(const Move& move, Board& board) {
    Position src {move.getSrc()};
    Position dst {move.getDst()};
    int srcX {src.getX()};
    int srcY {src.getY()};
    int dstX {dst.getX()};
    int dstY {dst.getY()};
    if (srcY < dstY) {
        Position castleFrom {srcX, board.getNumCols()-1};
        Position castleTo {srcX, dstY-1};
        Move castleMove {castleFrom, castleTo};
        setMoved(castleMove, board);
        board.movePiece(castleMove);
    }
    else {
        Position castleFrom {srcX, 0};
        Position castleTo {srcX, dstY+1};
        Move castleMove {castleFrom, castleTo};
        setMoved(castleMove, board);
        board.movePiece(castleMove);
    }
}

void promotePawn(const Move& move, Board& board, UserIO& io, InputParser& parser) {
    std::string in {io.promptPawnPromotion()};
    try {
        PieceType promotion {parser.parsePromotionCmd(in)};
        Position dst {move.getDst()};
        Piece* piece {board.getPiece(dst)};
        Color color {piece->getColor()};
        Piece* promote {nullptr};
        if (promotion == PAWN) {
            promote = new Pawn {color};
        }
        else if (promotion == KNIGHT) {
            promote = new Knight {color};
        }
        else if (promotion == BISHOP) {
            promote = new Bishop {color};
        }
        else if (promotion == ROOK) {
            promote = new Rook {color};
        }
        else if (promotion == QUEEN) {
            promote = new Queen {color};
        }
        board.setPiece(dst, promote);
    } catch (char const* msg) {
        io.printException(msg);
        promotePawn(move, board, io, parser);
    }
}

bool isCastlingMove(const Move& move, const Board& board) {
    Position src {move.getSrc()};
    Position dst {move.getDst()};
    Piece* piece {board.getPiece(dst)};
    if (piece && piece->getType() != KING) return false;
    if (abs(src.getY() - dst.getY()) != 2) return false;
    return true;
}

bool isPawnPromotion(const Move& move, const Board& board) {
    Position dst {move.getDst()};
    Piece* piece {board.getPiece(dst)};
    if (piece && piece->getType() != PAWN) return false;
    if (dst.getX() > 0 && dst.getX() < board.getNumRows()-1) return false;
    return true;
}

int main() {
    Board board {};
    Evaluator evaluator {};
    GameState state {evaluator.evaluateBoard(board)};
    UserIO io {};
    InputParser parser {};
    bool whiteToMove {true};
    while (state != CHECK_MATE && state != STALE_MATE) {
        try {
            io.printBoard(board);
            std::string in {io.promptInput(whiteToMove)};
            Move move {parser.parseMoveCmd(in)};
            if (!evaluator.isLegalMove(move, board, (whiteToMove ? WHITE : BLACK))) {
                io.printIllegalMoveMsg();
                continue;
            }
            setMoved(move, board);
            board.movePiece(move);
            if (isCastlingMove(move, board)) {
                castle(move, board);
            }
            if (isPawnPromotion(move, board)) {
                promotePawn(move, board, io, parser);
            }
            whiteToMove = !whiteToMove;
            state = evaluator.evaluateBoard(board);
            if (state == IN_CHECK) {
                io.printCheckMsg();
                state = IN_PROGRESS;
            }
        } catch (char const* msg) {
            io.printException(msg);
        }
    }
    if (state == CHECK_MATE) {
        io.printCheckMateMsg();
    }
    else if (state == STALE_MATE) {
        io.printStaleMateMsg();
    }
    board.destroy();
    return 0;
}
