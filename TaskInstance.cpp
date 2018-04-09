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
    madeMoves = old.madeMoves;

    memcpy(board, old.board, sizeof (char) * ARR_INIT_SIZE * ARR_INIT_SIZE);
}

void TaskInstance::printTaskInfo(int &k, int &h) {
    std::cout << "k: " << k << ", h: " << h << std::endl;
    std::cout << "queen position: " << queenPosition.first << ", " << queenPosition.second << std::endl;
    std::cout << "# of black peons: " << blacksCount << std::endl;
    std::cout << "# of moves: " << movesCount << std::endl;
    printBoard(k);
}

void TaskInstance::printBoard(int &k) {
    cout << "board:" << endl;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            cout << board[j][i];
        }
        cout << endl;
    }
}

void TaskInstance::readFromFile(const string &filename, int &k, int &h) {
    std::ifstream input(filename);

    input >> k >> h;
    std::string line;
    getline(input, line);

    for (int i = 0; i < k; i++) {
        getline(input, line);
        for (int j = 0; j < k; j++) {
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
void TaskInstance::getPossibleMoves(int const &k, vector<pair<int, int>> &possibleMoves){

    // go left
    for (int x = queenPosition.first - 1; x >= 0; x--) {
        if (!shouldMoveFurther(x, queenPosition.second, possibleMoves)) break;
    }

    // go right
    for (int x = queenPosition.first + 1; x < k; x++) {
        if (!shouldMoveFurther(x, queenPosition.second, possibleMoves)) break;
    }

    // go up
    for (int y = queenPosition.second - 1; y >= 0; y--) {
        if (!shouldMoveFurther(queenPosition.first, y, possibleMoves)) break;
    }

    // go down
    for (int y = queenPosition.second + 1; y < k; y++) {
        if (!shouldMoveFurther(queenPosition.first, y, possibleMoves)) break;
    }

    // go up the top-left diagonal
    {
        int x = queenPosition.first - 1;
        int y = queenPosition.second - 1;

        while (x >= 0 && y >= 0) {
            if (!shouldMoveFurther(x, y, possibleMoves)) break;

            x--;
            y--;
        }
    }


    // go down the top-left diagonal
    {
        int x = queenPosition.first + 1;
        int y = queenPosition.second + 1;

        while (x < k && y < k) {
            if (!shouldMoveFurther(x, y, possibleMoves)) break;

            x++;
            y++;
        }
    }

    // go down the top-right diagonal
    {
        int x = queenPosition.first - 1;
        int y = queenPosition.second + 1;

        while (x >= 0 && y < k) {
            if (!shouldMoveFurther(x, y, possibleMoves)) break;

            x--;
            y++;
        }
    }

    // go up the top-right diagonal
    {
        int x = queenPosition.first + 1;
        int y = queenPosition.second - 1;

        while (x < k && y >= 0) {
            if (!shouldMoveFurther(x, y, possibleMoves)) break;

            x++;
            y--;
        }
    }
}

bool TaskInstance::shouldMoveFurther(int const &x, int const &y, vector<pair<int, int> > &possibleMoves) {
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