//
// Created by akarola on 012 12. 3. 18.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>

using namespace std;

#ifndef CV1_BOARD_H
#define CV1_BOARD_H


static const int ARR_INIT_SIZE = 10;

static const char EMPTY_SQUARE = '0';
static const char BLACK_PEON = '1';
static const char WHITE_PEON = '2';
static const char QUEEN = '3';


class TaskInstance {
public:
    TaskInstance();
    TaskInstance(const TaskInstance &old);

    int blacksCount; // sum of present black peons

    int queenPosition[2];
    char board[ARR_INIT_SIZE][ARR_INIT_SIZE];
    vector<pair<int, int>> madeMoves;

    void readFromFile(const string &filename, int &k, int &h);
    void getPossibleMoves(int const &k, vector<pair<int, int>> &possibleMoves);

    void printTaskInfo(int &k, int &h);
    void printBoard(int &k);

private:
    bool shouldMoveFurther(int const &x, int const &y, vector<pair<int, int>> &possibleMoves);
};


#endif //CV1_BOARD_H
