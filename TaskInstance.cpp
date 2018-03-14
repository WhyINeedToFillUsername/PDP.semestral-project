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

    queenPosition.first = old.queenPosition.first;
    queenPosition.second = old.queenPosition.second;
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
    for (short i = 0; i < k; i++) {
        for (short j = 0; j < k; j++) {
            cout << board[j][i];
        }
        cout << endl;
    }
}

void TaskInstance::readFromFile(const string &filename) {
    std::ifstream input(filename);

    input >> k >> h;
    std::string line;
    getline(input, line);

    for (short i = 0; i < k; i++) {
        getline(input, line);
        vector<short> row;
        for (short j = 0; j < k; j++) {
            short figure = line.at(j) - '0';
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
vector<pair<short, short>> TaskInstance::getPossibleMoves() {
    vector<pair<short, short>> possibleMoves = vector<pair<short, short>>();

    // go left
    for (short x = queenPosition.first - 1; x >= 0; x--) {
        if (!shouldMoveFurther(x, queenPosition.second, possibleMoves)) break;
    }

    // go right
    for (short x = queenPosition.first + 1; x < k; x++) {
        if (!shouldMoveFurther(x, queenPosition.second, possibleMoves)) break;
    }

    // go up
    for (short y = queenPosition.second - 1; y >= 0; y--) {
        if (!shouldMoveFurther(queenPosition.first, y, possibleMoves)) break;
    }

    // go down
    for (short y = queenPosition.second + 1; y < k; y++) {
        if (!shouldMoveFurther(queenPosition.first, y, possibleMoves)) break;
    }

    // go up the top-left diagonal
    {
        short x = queenPosition.first - 1;
        short y = queenPosition.second - 1;

        while (x >= 0 && y >= 0) {
            if (!shouldMoveFurther(x, y, possibleMoves)) break;

            x--;
            y--;
        }
    }


    // go down the top-left diagonal
    {
        short x = queenPosition.first + 1;
        short y = queenPosition.second + 1;

        while (x < k && y < k) {
            if (!shouldMoveFurther(x, y, possibleMoves)) break;

            x++;
            y++;
        }
    }

    // go down the top-right diagonal
    {
        short x = queenPosition.first - 1;
        short y = queenPosition.second + 1;

        while (x >= 0 && y >= 0) {
            if (!shouldMoveFurther(x, y, possibleMoves)) break;

            x--;
            y++;
        }
    }

    // go up the top-right diagonal
    {
        short x = queenPosition.first + 1;
        short y = queenPosition.second - 1;

        while (x >= 0 && y >= 0) {
            if (!shouldMoveFurther(x, y, possibleMoves)) break;

            x++;
            y--;
        }
    }

    return possibleMoves;
}

bool TaskInstance::shouldMoveFurther(short x, short y, vector<pair<short, short>> &possibleMoves) {
    if (x < 0 || y < 0 || x >= k || y >= k) return false;
    short itemAtNewPosition = board[x][y];

    if (itemAtNewPosition == EMPTY_SQUARE) {
        possibleMoves.emplace_back(x, y);
        return true;
    } else if (itemAtNewPosition == BLACK_PEON) {
        possibleMoves.insert(possibleMoves.begin(), make_pair(x, y));
        return false;
    } else {
        return false;
    }
}