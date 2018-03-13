//
// Created by akarola on 012 12. 3. 18.
//
#include "TaskInstance.h"

TaskInstance::TaskInstance() {
    movesCount = 0;
    blacksCount = 0;
}

// copy constructor
TaskInstance::TaskInstance(const TaskInstance &old) {
//    cout << "copy constructor called" << endl;

    k = old.k;
    h = old.h;

    movesCount = old.movesCount;
    blacksCount = old.blacksCount;

    queenPosition = old.queenPosition;
    board = old.board;
}

void TaskInstance::printTaskInfo() {
    std::cout << "k: " << k << ", h: " << h << std::endl;
    std::cout << "queen position: " << queenPosition.first << ", " << queenPosition.second << std::endl;
    std::cout << "# of black peons: " << blacksCount << std::endl;
    std::cout << "# of moves: " << movesCount << std::endl;
    printBoard();
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
                this->queenPosition = make_pair(i, j);
            } else if (figure == BLACK_PEON) {
                this->blacksCount++;
            }
            row.push_back(figure);
//            cout << line.at(j);
        }
        this->board.push_back(row);
//        cout << std::endl;
    }
//    cout << std::endl;
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
    for (int x = queenPosition.first - 1; x >= 0; x--) {
        int itemAtNewPosition = board[x][queenPosition.second];

        if (itemAtNewPosition == EMPTY_SQUARE) {
            possibleMoves.emplace_back(x, queenPosition.second);
        } else if (itemAtNewPosition == BLACK_PEON) {
            possibleMoves.insert(possibleMoves.begin(), make_pair(x, queenPosition.second));
            break;
        } else {
            break;
        }
    }

    // go right
    for (int x = queenPosition.first + 1; x < k; x++) {
        int itemAtNewPosition = board[x][queenPosition.second];

        if (itemAtNewPosition == EMPTY_SQUARE) {
            possibleMoves.emplace_back(x, queenPosition.second);
        } else if (itemAtNewPosition == BLACK_PEON) {
            possibleMoves.insert(possibleMoves.begin(), make_pair(x, queenPosition.second));
            break;
        } else {
            break;
        }
    }

    // go up
    for (int y = queenPosition.second - 1; y >= 0; y--) {
        int itemAtNewPosition = board[queenPosition.first][y];

        if (itemAtNewPosition == EMPTY_SQUARE) {
            possibleMoves.emplace_back(queenPosition.first, y);
        } else if (itemAtNewPosition == BLACK_PEON) {
            possibleMoves.insert(possibleMoves.begin(), make_pair(queenPosition.first, y));
            break;
        } else {
            break;
        }
    }

    // go down
    for (int y = queenPosition.second + 1; y < k; y++) {
        int itemAtNewPosition = board[queenPosition.first][y];

        if (itemAtNewPosition == EMPTY_SQUARE) {
            possibleMoves.emplace_back(queenPosition.first, y);
        } else if (itemAtNewPosition == BLACK_PEON) {
            possibleMoves.insert(possibleMoves.begin(), make_pair(queenPosition.first, y));
            break;
        } else {
            break;
        }
    }

    // go up the top-left diagonal
    {
        int x = queenPosition.first - 1;
        int y = queenPosition.second - 1;

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
        int x = queenPosition.first + 1;
        int y = queenPosition.second + 1;

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
        int x = queenPosition.first - 1;
        int y = queenPosition.second + 1;

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
        int x = queenPosition.first + 1;
        int y = queenPosition.second - 1;

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
