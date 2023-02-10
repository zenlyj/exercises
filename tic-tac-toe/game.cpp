#include <iostream>
#include <vector>
#include "string"
#include <limits.h>

enum Piece {
    CROSS = static_cast<int>('X'),
    CIRCLE = static_cast<int>('O'),
    EMPTY = static_cast<int>(' ')
};

enum GameState {
    AI_WIN,
    PLAYER_WIN,
    TIE,
    IN_PROGRESS
};

const std::string tieMsg {"GAME TIE!"};
const std::string playerWinMsg {"PLAYER WINS!"};
const std::string AIWinMsg {"AI WINS!"};
const std::string placementPrompt {"Enter placement row & col: (eg. 0,1)"};
const std::string invalidPlacementPrompt {"Invalid user input!"};

const std::string rowOutOfBoundsPrompt {"Row out of bounds!"};
const std::string colOutOfBoundsPrompt {"Column out of bounds!"};
const std::string spaceFilledPrompt {"Space is filled!"};

class Placement {
    private:
        int row {};
        int col {};

    public:
        Placement() {}

        Placement(int m_row, int m_col) : row{m_row}, col{m_col} {}

        int getRow() {
            return row;
        }

        int getCol() {
            return col;
        }
};

class Board {
    private:
        const int numRows {3};
        const int numCols {3};
        std::vector<std::vector<char>> state {};

    public:
        Board() {
            for (int i = 0; i < numRows; i++) {
                std::vector<char> row {};
                for (int j = 0; j < numCols; j++) {
                    row.push_back(EMPTY);
                }
                state.push_back(row);
            }
        }

        void setPiece(Piece piece, Placement& placement) {
            int row = placement.getRow();
            int col = placement.getCol();
            if (row < 0 || row >= numRows) {
                throw rowOutOfBoundsPrompt;
            }
            if (col < 0 || col >= numCols) {
                throw colOutOfBoundsPrompt;
            }
            if (state[row][col] == CROSS || state[row][col] == CIRCLE) {
                throw spaceFilledPrompt;
            }
            state[row][col] = static_cast<char>(piece);
        }

        std::vector<std::vector<char>> getState() {
            return state;
        }

        bool isFull() {
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
                    if (state[i][j] == EMPTY) {
                        return false;
                    }
                }
            }
            return true;
        }

        int getNumRow() {
            return numRows;
        }

        int getNumCol() {
            return numCols;
        }

        std::string toString() {
            std::string s_state {};
            for (int i = 0; i < numRows; i++) {
                s_state.push_back('\t');
                for (int j = 0; j < numCols; j++) {
                    s_state.push_back(state[i][j]);
                    s_state.append(j < numCols-1 ? " | " : "");
                }
                s_state.append("\n\t---------\n");
            }
            return s_state;
        }
};

class Evaluator {
    private:
        GameState horizontalCheck(Board& board, std::vector<std::vector<char>>& state) {
            for (int i = 0; i < board.getNumRow(); i++) {
                int j {0};
                char curr {state[i][j++]};
                bool isSame {curr != EMPTY};
                for (; j < board.getNumCol(); j++) {
                    char next = state[i][j];
                    isSame &= next != EMPTY && next == curr;
                }
                if (isSame) {
                    return curr == CROSS ? AI_WIN : PLAYER_WIN;
                }
            }
            return IN_PROGRESS;
        }

        GameState verticalCheck(Board& board, std::vector<std::vector<char>>& state) {
            for (int j = 0; j < board.getNumCol(); j++) {
                int i {0};
                char curr {state[i++][j]};
                bool isSame {curr != EMPTY};
                for (; i < board.getNumRow(); i++) {
                    char next = state[i][j];
                    isSame &= next != EMPTY && next == curr;
                }
                if (isSame) {
                    return curr == CROSS ? AI_WIN : PLAYER_WIN;
                }
            }
            return IN_PROGRESS;
        }

        GameState diagonalChecks(Board& board, std::vector<std::vector<char>>& state) {
            int i {0};
            char curr {state[i][i]};
            bool isSame {curr != EMPTY};
            for (i += 1; i < board.getNumRow(); i++) {
                char next = state[i][i];
                isSame &= next != EMPTY && next == curr; 
            }
            if (isSame) {
                return curr == CROSS ? AI_WIN : PLAYER_WIN;
            }

            i = 0;
            curr = state[i++][board.getNumCol()-1];
            isSame = {curr != EMPTY};
            for (; i < board.getNumRow(); i++) {
                char next = state[i][board.getNumRow()-1-i];
                isSame &= next != EMPTY && next == curr;
            }
            if (isSame) {
                return curr == CROSS ? AI_WIN : PLAYER_WIN;
            }
            return IN_PROGRESS;
        }

    public:
        GameState evaluate(Board board) {
            std::vector<std::vector<char>> state {board.getState()};
            GameState status {};
            status = horizontalCheck(board, state);
            if (status != IN_PROGRESS) return status;

            status = verticalCheck(board, state);
            if (status != IN_PROGRESS) return status;

            status = diagonalChecks(board, state);
            if (status != IN_PROGRESS) return status;

            return board.isFull() ? TIE : IN_PROGRESS;   
        }
};

class AI {
    private:
        Evaluator evaluator {};

        std::vector<Placement> getPossibles(Board board) {
            std::vector<std::vector<char>> state {board.getState()};
            std::vector<Placement> possibles {};
            for (int i = 0; i < state.size(); i++) {
                for (int j = 0; j < state[0].size(); j++) {
                    if (state[i][j] == EMPTY) {
                        possibles.push_back(Placement{i, j});
                    }
                }
            }
            return possibles;
        }
        
        int miniMax(Board board, bool inTurn) {
            GameState gameState {evaluator.evaluate(board)};
            if (gameState == AI_WIN) {
                return 10;
            }
            if (gameState == PLAYER_WIN) {
                return -10;
            }
            if (gameState == TIE) {
                return 0;
            }
            std::vector<Placement> possibles {getPossibles(board)};
            if (inTurn) {
                // maximizing
               int maxEval {INT_MIN};
               for (Placement placement : possibles) {
                    Board nextBoard = board;
                    nextBoard.setPiece(CROSS, placement);
                    maxEval = std::max(maxEval, miniMax(nextBoard, !inTurn));
               }
               return maxEval;
            }
            else {
                // minimizing
                int minEval {INT_MAX};
                for (Placement placement : possibles) {
                    Board nextBoard = board;
                    nextBoard.setPiece(CIRCLE, placement);
                    minEval = std::min(minEval, miniMax(nextBoard, !inTurn));
                }
                return minEval;
            }
        }

    public:
        AI(Evaluator m_evaluator) : evaluator{evaluator} {}

        Placement getNextMove(Board board) {
            std::vector<Placement> possibles {getPossibles(board)};
            int bestScore {INT_MIN};
            Placement res {};
            for (Placement placement : possibles) {
                Board nextBoard = board;
                nextBoard.setPiece(CROSS, placement);
                int score {miniMax(nextBoard, false)};
                if (score > bestScore) {
                    bestScore = score;
                    res = placement;
                }
            }
            return res;
        }
};

class Game {
    private:
        Board board {};
        Evaluator evaluator {};
        AI bot {evaluator};

    public:
        void start() {
            GameState gameState = {evaluator.evaluate(board)};
            bool botTurn {true}; 
            while (gameState == IN_PROGRESS) {
                try {
                    Piece piece {botTurn ? CROSS : CIRCLE};
                    Placement nextMove {botTurn ? bot.getNextMove(board) : getPlacement()};
                    board.setPiece(piece, nextMove);
                    botTurn = !botTurn;
                    std:: cout << board.toString() << '\n';
                    gameState = evaluator.evaluate(board);
                } catch (char const* msg) {
                    std::cout << msg << "\n";
                }
            }
            if (gameState == TIE) {
                std::cout << tieMsg << '\n';
            }
            else if (gameState == PLAYER_WIN) {
                std::cout << playerWinMsg << '\n';
            }
            else if (gameState == AI_WIN) {
                std::cout << AIWinMsg << '\n';
            }
        }

        Placement getPlacement() {
            std::string in {};
            std::cout << placementPrompt << '\n';
            std::getline(std::cin >> std::ws, in);
            
            std::vector<int> lst {};
            for (int i = 0; i < in.size(); i++) {
                char c = in.at(i);
                if (isspace(c) || c == ',') {
                    continue;
                }
                else if (isdigit(c)) {
                    lst.push_back(c-'0');
                }
                else {
                    break;
                }
            }
            
            if (lst.size() != 2) {
                throw invalidPlacementPrompt;
            }

            return Placement{lst[0], lst[1]};
        }
};

int main() {
    Game game {};
    game.start();
    return 0;
}
