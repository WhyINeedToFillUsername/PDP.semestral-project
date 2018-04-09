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

    movesCount = old.movesCount;
    blacksCount = old.blacksCount;

    queenPosition.first = old.queenPosition.first;
    queenPosition.second = old.queenPosition.second;
    memcpy(board, old.board, sizeof (char) * ARR_INIT_SIZE * ARR_INIT_SIZE);
}

void TaskInstance::printTaskInfo(short &k, short &h) {
    std::cout << "k: " << k << ", h: " << h << std::endl;
    std::cout << "queen position: " << queenPosition.first << ", " << queenPosition.second << std::endl;
    std::cout << "# of black peons: " << blacksCount << std::endl;
    std::cout << "# of moves: " << movesCount << std::endl;
    printBoard(k);
}

void TaskInstance::printBoard(short &k) {
    cout << "board:" << endl;
    for (short i = 0; i < k; i++) {
        for (short j = 0; j < k; j++) {
            cout << board[j][i];
        }
        cout << endl;
    }
}

void TaskInstance::readFromFile(const string &filename, short &k, short &h) {
    std::ifstream input(filename);

    input >> k >> h;
    std::string line;
    getline(input, line);

    for (short i = 0; i < k; i++) {
        getline(input, line);
        for (short j = 0; j < k; j++) {
            char figure = line.at(j);
            if (figure == QUEEN) {
                this->queenPosition.first = i;
                this->queenPosition.second = j;
            } else if (figure == BLACK_PEON) {
                this->blacksCount++;
            }
            board[i][j] = figure;
//            cout << line.at(j);
        }
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
vector<pair<short, short>> TaskInstance::getPossibleMoves(short const &k) {
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

        while (x >= 0 && y < k) {
            if (!shouldMoveFurther(x, y, possibleMoves)) break;

            x--;
            y++;
        }
    }

    // go up the top-right diagonal
    {
        short x = queenPosition.first + 1;
        short y = queenPosition.second - 1;

        while (x < k && y >= 0) {
            if (!shouldMoveFurther(x, y, possibleMoves)) break;

            x++;
            y--;
        }
    }

    return possibleMoves;
}

bool TaskInstance::shouldMoveFurther(short const &x, short const &y, vector<pair<short, short> > &possibleMoves) {
    const char itemAtNewPosition = board[x][y];

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