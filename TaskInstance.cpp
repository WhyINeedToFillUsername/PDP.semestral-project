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

    queenPosition[0] = old.queenPosition[0];
    queenPosition[1] = old.queenPosition[1];
    madeMoves = old.madeMoves;

    memcpy(board, old.board, sizeof (char) * ARR_INIT_SIZE * ARR_INIT_SIZE);
}

// constructor from existing data
TaskInstance::TaskInstance(int &movesCount, int &blacksCount, const int *queenPosition, char *board, int moves[][2]) {
//    cout << "copy constructor called" << endl;

    this->movesCount = movesCount;
    this->blacksCount = blacksCount;

    this->queenPosition[0] = queenPosition[0];
    this->queenPosition[1] = queenPosition[1];

    // arr to vector of pairs
    for (int i = 0; i < movesCount; i++) {
        this->madeMoves.emplace_back(moves[i][0], moves[i][1]);
    }

    memcpy(this->board, board, sizeof (char) * ARR_INIT_SIZE * ARR_INIT_SIZE);
}

void TaskInstance::printTaskInfo(int &k, int &h) {
    std::cout << "k: " << k << ", h: " << h << std::endl;
    std::cout << "queen position: " << queenPosition[0] << ", " << queenPosition[1] << std::endl;
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
                this->queenPosition[0] = i;
                this->queenPosition[1] = j;
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
    for (int x = queenPosition[0] - 1; x >= 0; x--) {
        if (!shouldMoveFurther(x, queenPosition[1], possibleMoves)) break;
    }

    // go right
    for (int x = queenPosition[0] + 1; x < k; x++) {
        if (!shouldMoveFurther(x, queenPosition[1], possibleMoves)) break;
    }

    // go up
    for (int y = queenPosition[1] - 1; y >= 0; y--) {
        if (!shouldMoveFurther(queenPosition[0], y, possibleMoves)) break;
    }

    // go down
    for (int y = queenPosition[1] + 1; y < k; y++) {
        if (!shouldMoveFurther(queenPosition[0], y, possibleMoves)) break;
    }

    // go up the top-left diagonal
    {
        int x = queenPosition[0] - 1;
        int y = queenPosition[1] - 1;

        while (x >= 0 && y >= 0) {
            if (!shouldMoveFurther(x, y, possibleMoves)) break;

            x--;
            y--;
        }
    }


    // go down the top-left diagonal
    {
        int x = queenPosition[0] + 1;
        int y = queenPosition[1] + 1;

        while (x < k && y < k) {
            if (!shouldMoveFurther(x, y, possibleMoves)) break;

            x++;
            y++;
        }
    }

    // go down the top-right diagonal
    {
        int x = queenPosition[0] - 1;
        int y = queenPosition[1] + 1;

        while (x >= 0 && y < k) {
            if (!shouldMoveFurther(x, y, possibleMoves)) break;

            x--;
            y++;
        }
    }

    // go up the top-right diagonal
    {
        int x = queenPosition[0] + 1;
        int y = queenPosition[1] - 1;

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