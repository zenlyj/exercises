#include "Board.h"

Board::Board() {
    for (int i = 0; i < numRows; i++) {
        std::vector<Piece*> row {};
        for (int j = 0; j < numCols; j++) {
            row.push_back(nullptr);
        }
        mState.push_back(row);
    }

    for (auto& pos : initialPawnPos) {
        mState[pos[0]][pos[1]] = new Pawn {WHITE};
        mState[numRows-1-pos[0]][pos[1]] = new Pawn {BLACK};
    }

    for (auto& pos : initialKnightPos) {
        mState[pos[0]][pos[1]] = new Knight {WHITE};
        mState[numRows-1-pos[0]][pos[1]] = new Knight {BLACK};
    }

    for (auto& pos : initialBishopPos) {
        mState[pos[0]][pos[1]] = new Bishop {WHITE};
        mState[numRows-1-pos[0]][pos[1]] = new Bishop {BLACK};
    }

    for (auto& pos : initialRookPos) {
        mState[pos[0]][pos[1]] = new Rook {WHITE};
        mState[numRows-1-pos[0]][pos[1]] = new Rook {BLACK};
    }

    for (auto& pos : initialKingPos) {
        mState[pos[0]][pos[1]] = new King {WHITE};
        mState[numRows-1-pos[0]][pos[1]] = new King {BLACK};
    }

    for (auto& pos : initialQueenPos) {
        mState[pos[0]][pos[1]] = new Queen {WHITE};
        mState[numRows-1-pos[0]][pos[1]] = new Queen {BLACK};
    }
}

void Board::destroy() {
    for (auto& row : mState) {
        for (auto& piece : row) {
            delete piece;
        }
    }
    for (auto& piece : captured) {
        delete piece;
    }
}

Piece* Board::getPiece(const Position& pos) const {
    int x {pos.getX()};
    int y {pos.getY()};
    return mState[x][y];
}

void Board::setPiece(const Position& pos, Piece* piece) {
    int x {pos.getX()};
    int y {pos.getY()};
    captured.insert(mState[x][y]);
    mState[x][y] = piece;
}

int Board::getNumRows() const {
    return numRows;
}

int Board::getNumCols() const {
    return numCols;
}

void Board::movePiece(const Move& move) {
    const Position& src {move.getSrc()};
    const Position& dst {move.getDst()};
    Piece* srcPiece {mState[src.getX()][src.getY()]};
    Piece* dstPiece {mState[dst.getX()][dst.getY()]};
    mState[src.getX()][src.getY()] = nullptr;
    if (dstPiece) {
        captured.insert(dstPiece);
    }
    mState[dst.getX()][dst.getY()] = srcPiece;
}

const std::string Board::toString() const {
    std::string res {};
    res.append("\t   ---------------------------------\n");
    for (int i = 0; i < numRows; i++) {
        res.push_back('\t');
        res.push_back(static_cast<char>('1'+i));
        res.append("  | ");
        for (int j = 0; j < numCols; j++) {
            Piece* piece {mState[i][j]};
            if (!piece) {
                res.push_back(' ');
            }
            else {
                PieceType type {piece->getType()};
                char symbol {static_cast<char>(type)};
                res.push_back(piece->getColor() == WHITE ? symbol : static_cast<char>(symbol+32));
            }
            res.append(" | ");
        }
        res.push_back('\n');
        res.append("\t   ---------------------------------\n");
    }

    res.append("\t     ");
    for (int i = 0; i < numCols; i++) {
        res.push_back(static_cast<char>('A'+i));
        res.append("   ");
    }
    return res;
}
