#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <optional>
#include <map>

const int numRows {9};
const int numCols {9};
const int numSubRows {3};
const int numSubCols {3};
const std::set<char> boardVals {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
const char emptyVal {'0'};
std::vector<std::vector<char>> pre {
    {'0', '0', '0', '6', '0', '2', '4', '0', '0'},
    {'0', '0', '0', '8', '0', '0', '9', '0', '5'},
    {'0', '0', '4', '0', '9', '0', '0', '1', '0'},
    {'0', '1', '0', '0', '0', '8', '0', '0', '6'},
    {'6', '0', '0', '0', '0', '0', '0', '0', '2'},
    {'7', '0', '0', '4', '0', '0', '0', '3', '0'},
    {'0', '9', '0', '0', '5', '0', '8', '0', '0'},
    {'4', '0', '5', '0', '0', '7', '0', '0', '0'},
    {'0', '0', '6', '9', '0', '3', '0', '0', '0'}
};

std::set<char> setComplement(const std::set<char>& universe, std::set<char>& set) {
    std::set<char> res {};
    for (char c : universe) {
        if (set.find(c) == set.end()) {
            res.insert(c);
        }
    }
    return res;
}

std::set<char> setIntersection(std::set<char>& s1, std::set<char>& s2) {
    std::set<char> res {};
    for (char c1 : s1) {
        for (char c2 : s2) {
            if (c1 == c2) {
                res.insert(c1);
            }
        }
    }
    return res;
}

int subRowStart(int row) {
    int i {0};
    int res {i};
    while (i <= row) {
        res = i;
        i += numSubRows;
    } 
    return res;
}

int subColStart(int col) {
    int res {};
    int i {0};
    while (i <= col) {
        res = i;
        i += numSubCols;
    }
    return res;
}

std::set<char> getPossibleVals(std::vector<std::vector<char>>& state, int row, int col) {
    std::set<char> rowVals {};
    std::set<char> colVals {};
    std::set<char> subVals {};

    for (int i = 0; i < numCols; i++) {
        if (boardVals.find(state[row][i]) != boardVals.end()) {
            rowVals.insert(state[row][i]);
        }
    }
    for (int i = 0; i < numRows; i++) {
        if (boardVals.find(state[i][col]) != boardVals.end()) {
            colVals.insert(state[i][col]);
        }
    }
            
    int rowStart {subRowStart(row)};
    int colStart {subColStart(col)};

    for (int i = rowStart; i < rowStart+numSubRows; i++) {
        for (int j = colStart; j < colStart+numSubCols; j++) {
            if (boardVals.find(state[i][j]) != boardVals.end()) {
                subVals.insert(state[i][j]);
            }
        }
    }
    std::set<char> rowPossibles = setComplement(boardVals, rowVals);
    std::set<char> colPossibles = setComplement(boardVals, colVals);
    std::set<char> subPossibles = setComplement(boardVals, subVals);

    std::set<char> rowColIntersect {setIntersection(rowPossibles, colPossibles)};
    return setIntersection(subPossibles, rowColIntersect);
}

class Board {
    private:
        std::vector<std::vector<char>> state {};

    public:
        Board(std::vector<std::vector<char>>& state) {
            this->state = state;
        }

        void setVal(int row, int col, char val) {
            state[row][col] = val;

        }

        bool isFilled() {
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
                    if (state[i][j] == emptyVal) {
                        return false;
                    }
                }
            }
            return true;
        }

        std::vector<std::vector<char>> getState() {
            return state;
        }

        std::string toString() {
            std::string res {};
            for (int i = 0; i < numRows; i++) {
                res.push_back('\t');
                for (int j = 0; j < numCols; j++) {
                    res.push_back(state[i][j]);
                    res.append(" | "); 
                }
                res.push_back('\n');
                res.append("\t-----------------------------------\n");
            }
            return res;
        }
};

class Solver {
    private:
        void constrain(std::map<std::pair<int, int>, std::set<char>>& possibles, std::vector<std::pair<int, int>>& emptys, int row, int col, char val) {
            for (auto& empty : emptys) {
                if (empty.first == row || empty.second == col) {
                    possibles[empty].erase(val);
                }
                int rowStart {subRowStart(row)};
                int colStart {subColStart(col)};

                for (int i = rowStart; i < rowStart+numSubRows; i++) {
                    for (int j = colStart; j < colStart+numSubCols; j++) {
                        if (empty.first == i && empty.second == j) {
                            possibles[empty].erase(val);
                        }
                    }
                }
            }
        }

        bool helper(Board& board, std::map<std::pair<int, int>, std::set<char>>& possibles, std::vector<std::pair<int, int>>& emptys, int curr) {
            if (board.isFilled()) {
                return true;
            }
            std::pair<int, int> empty {emptys[curr]};
            for (auto& c : possibles[empty]) {
                board.setVal(empty.first, empty.second, c);
                std::map<std::pair<int, int>, std::set<char>> newPossibles = possibles;
                constrain(newPossibles, emptys, empty.first, empty.second, c);
                if (helper(board, newPossibles, emptys, curr+1)) {
                    return true;
                }
                board.setVal(empty.first, empty.second, '0');
            }
            return false;
        }
    
    public:
        bool solve(Board& board) {
            std::map<std::pair<int, int>, std::set<char>> possibles {};
            std::vector<std::pair<int, int>> emptys {};
            std::vector<std::vector<char>> state {board.getState()};
            
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
                    if (state[i][j] == emptyVal) {
                        std::set<char> vals {getPossibleVals(state, i, j)};
                        emptys.push_back(std::make_pair(i, j));
                    }
                }
            }
            for (auto& p : emptys) {
                int i {p.first};
                int j {p.second};
                std::set<char> vals {getPossibleVals(state, i, j)};
                possibles.emplace(p, vals);
            }

            return helper(board, possibles, emptys, 0);
        }
};

int main() {
    Board board {pre};
    Solver solver {};
    solver.solve(board);
    std::cout << board.toString() << '\n';
    return 0;
}