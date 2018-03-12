//
// Created by akarola on 012 12. 3. 18.
//
#include "TaskInstance.h"

TaskInstance::TaskInstance() {
    moves = 0;
    blacksCount = 0;
}

void TaskInstance::printInfo() {
    std::cout << "k: " << k << ", h: " << h << std::endl;
    std::cout << "queen position: " << queen.first << ", " << queen.second << std::endl;
    std::cout << "# of black peons: " << blacksCount << std::endl;
    std::cout << "# of moves: " << moves << std::endl;
}

void TaskInstance::printBoard() {
    cout << "board:" << endl;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            cout << board[j][i];
        }
        cout << endl;
    }
}

void TaskInstance::readFromFile(const char *filename) {
    std::ifstream input(filename);

    input >> k >> h;
    std::string line;
    getline(input, line);

    for (int i = 0; i < k; i++) {
        getline(input, line);
        vector<int> row;
        for (int j = 0; j < k; j++) {
            int figure = line.at(j) - '0';
            if (figure == QUEEN) {
                this->queen = make_pair(i, j);
            } else if (figure == BLACK_PEON) {
                this->blacksCount++;
            }
            row.push_back(figure);
            cout << line.at(j);
        }
        this->board.push_back(row);
        cout << std::endl;
    }
    cout << std::endl;
}

/**
 * Finds all possible moves of the queen on the board. This method puts the possible positions with black peons first
 * (we prefer to visit these positions first).
 *
 * @return vector with positions of possible moves of the queen.
 */
vector<pair<int, int>> TaskInstance::getPossibleMoves() {
    vector<pair<int, int>> possibleMoves = vector<pair<int, int>>();

    // go left
    for (int x = queen.first - 1; x >= 0; x--) {
        int itemAtNewPosition = board[x][queen.second];

        if (itemAtNewPosition == EMPTY_SQUARE) {
            possibleMoves.emplace_back(x, queen.second);
        } else if (itemAtNewPosition == BLACK_PEON) {
            possibleMoves.insert(possibleMoves.begin(), make_pair(x, queen.second));
            break;
        } else {
            break;
        }
    }

    // go right
    for (int x = queen.first + 1; x < k; x++) {
        int itemAtNewPosition = board[x][queen.second];

        if (itemAtNewPosition == EMPTY_SQUARE) {
            possibleMoves.emplace_back(x, queen.second);
        } else if (itemAtNewPosition == BLACK_PEON) {
            possibleMoves.insert(possibleMoves.begin(), make_pair(x, queen.second));
            break;
        } else {
            break;
        }
    }

    // go up
    for (int y = queen.second - 1; y >= 0; y--) {
        int itemAtNewPosition = board[queen.first][y];

        if (itemAtNewPosition == EMPTY_SQUARE) {
            possibleMoves.emplace_back(queen.first, y);
        } else if (itemAtNewPosition == BLACK_PEON) {
            possibleMoves.insert(possibleMoves.begin(), make_pair(queen.first, y));
            break;
        } else {
            break;
        }
    }

    // go down
    for (int y = queen.second + 1; y < k; y++) {
        int itemAtNewPosition = board[queen.first][y];

        if (itemAtNewPosition == EMPTY_SQUARE) {
            possibleMoves.emplace_back(queen.first, y);
        } else if (itemAtNewPosition == BLACK_PEON) {
            possibleMoves.insert(possibleMoves.begin(), make_pair(queen.first, y));
            break;
        } else {
            break;
        }
    }

    // go up the top-left diagonal
    {
        int x = queen.first - 1;
        int y = queen.second - 1;

        while (x >= 0 && y >= 0) {
            int itemAtNewPosition = board[x][y];
            if (itemAtNewPosition == EMPTY_SQUARE) {
                possibleMoves.emplace_back(x, y);
            } else if (itemAtNewPosition == BLACK_PEON) {
                possibleMoves.insert(possibleMoves.begin(), make_pair(x, y));
                break;
            } else {
                break;
            }
            x--;
            y--;
        }
    }


    // go down the top-left diagonal
    {
        int x = queen.first + 1;
        int y = queen.second + 1;

        while (x < k && y < k) {
            int itemAtNewPosition = board[x][y];
            if (itemAtNewPosition == EMPTY_SQUARE) {
                possibleMoves.emplace_back(x, y);
            } else if (itemAtNewPosition == BLACK_PEON) {
                possibleMoves.insert(possibleMoves.begin(), make_pair(x, y));
                break;
            } else {
                break;
            }
            x++;
            y++;
        }
    }

    // go down the top-right diagonal
    {
        int x = queen.first - 1;
        int y = queen.second + 1;

        while (x >= 0 && y >= 0) {
            int itemAtNewPosition = board[x][y];
            if (itemAtNewPosition == EMPTY_SQUARE) {
                possibleMoves.emplace_back(x, y);
            } else if (itemAtNewPosition == BLACK_PEON) {
                possibleMoves.insert(possibleMoves.begin(), make_pair(x, y));
                break;
            } else {
                break;
            }
            x--;
            y++;
        }
    }

    // go up the top-right diagonal
    {
        int x = queen.first + 1;
        int y = queen.second - 1;

        while (x >= 0 && y >= 0) {
            int itemAtNewPosition = board[x][y];
            if (itemAtNewPosition == EMPTY_SQUARE) {
                possibleMoves.emplace_back(x, y);
            } else if (itemAtNewPosition == BLACK_PEON) {
                possibleMoves.insert(possibleMoves.begin(), make_pair(x, y));
                break;
            } else {
                break;
            }
            x++;
            y--;
        }
    }

    return possibleMoves;
}

void TaskInstance::printPossibleMoves(vector<pair<int, int>> moves) {
    cout << "queen possible moves: ";
    for (auto &&item : moves) {
        cout << "(" << item.first << "," << item.second << "); ";
    }
    cout << endl;
}
