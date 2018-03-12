//
// Created by akarola on 012 12. 3. 18.
//
#include "TaskInstance.h"

TaskInstance::TaskInstance() {
}

void TaskInstance::printInfo() {
    std::cout << "k: " << k << ", h: " << h << std::endl;
    std::cout << "queen position: " << queen.first << ", " << queen.second << std::endl;
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
            }
            row.push_back(figure);
            cout << line.at(j);
        }
        this->board.push_back(row);
        cout << std::endl;
    }
    cout << std::endl;
}

vector<pair<int, int>> TaskInstance::getPossibleMoves() {
    vector<pair<int, int>> possibleMoves = vector<pair<int, int>>();

    // go left
    for (int i = queen.first - 1; i >= 0; i--) {
        int itemAtNewPosition = board[i][queen.second];

        if (itemAtNewPosition == EMPTY) {
            possibleMoves.emplace_back(i, queen.second);
        } else if (itemAtNewPosition == BLACK_PEON) {
            possibleMoves.emplace_back(i, queen.second);
            break;
        } else {
            break;
        }
    }

    // go right
    for (int i = queen.first + 1; i < k; i++) {
        int itemAtNewPosition = board[i][queen.second];

        if (itemAtNewPosition == EMPTY) {
            possibleMoves.emplace_back(i, queen.second);
        } else if (itemAtNewPosition == BLACK_PEON) {
            possibleMoves.emplace_back(i, queen.second);
            break;
        } else {
            break;
        }
    }

    // go up
    for (int i = queen.second - 1; i >= 0; i--) {
        int itemAtNewPosition = board[queen.first][i];

        if (itemAtNewPosition == EMPTY) {
            possibleMoves.emplace_back(queen.first, i);
        } else if (itemAtNewPosition == BLACK_PEON) {
            possibleMoves.emplace_back(queen.first, i);
            break;
        } else {
            break;
        }
    }

    // go down
    for (int i = queen.second + 1; i < k; i++) {
        int itemAtNewPosition = board[queen.first][i];

        if (itemAtNewPosition == EMPTY) {
            possibleMoves.emplace_back(queen.first, i);
        } else if (itemAtNewPosition == BLACK_PEON) {
            possibleMoves.emplace_back(queen.first, i);
            break;
        } else {
            break;
        }
    }

    // go up the top-left diagonal
//    for (int i = queen.first - 1; i >= 0; i--) {
//        int itemAtNewPosition = board[i][queen.second];
//
//        if (itemAtNewPosition == EMPTY) {
//            possibleMoves.emplace_back(i, queen.second);
//        } else if (itemAtNewPosition == BLACK_PEON) {
//            possibleMoves.emplace_back(i, queen.second);
//            break;
//        } else {
//            break;
//        }
//    }

    // go down the top-left diagonal
//    for (int i = queen.first + 1; i < k; i++) {
//        int itemAtNewPosition = board[i][queen.second];
//
//        if (itemAtNewPosition == EMPTY) {
//            possibleMoves.emplace_back(i, queen.second);
//        } else if (itemAtNewPosition == BLACK_PEON) {
//            possibleMoves.emplace_back(i, queen.second);
//            break;
//        } else {
//            break;
//        }
//    }

    // go up the top-right diagonal
//    for (int i = queen.second - 1; i >= 0; i--) {
//        int itemAtNewPosition = board[queen.first][i];
//
//        if (itemAtNewPosition == EMPTY) {
//            possibleMoves.emplace_back(queen.first, i);
//        } else if (itemAtNewPosition == BLACK_PEON) {
//            possibleMoves.emplace_back(queen.first, i);
//            break;
//        } else {
//            break;
//        }
//    }

    // go down the top-right diagonal
//    for (int i = queen.second + 1; i < k; i++) {
//        int itemAtNewPosition = board[queen.first][i];
//
//        if (itemAtNewPosition == EMPTY) {
//            possibleMoves.emplace_back(queen.first, i);
//        } else if (itemAtNewPosition == BLACK_PEON) {
//            possibleMoves.emplace_back(queen.first, i);
//            break;
//        } else {
//            break;
//        }
//    }

    return possibleMoves;
}

void TaskInstance::printPossibleMoves(vector<pair<int, int>> moves) {
    cout << "queen possible moves: ";
    for (auto &&item : moves) {
        cout << "(" << item.first << "," << item.second << "); ";
    }
    cout << endl;
}
