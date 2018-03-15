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


static const short ARR_INIT_SIZE = 10;

static const char EMPTY_SQUARE = '0';
static const char BLACK_PEON = '1';
static const char WHITE_PEON = '2';
static const char QUEEN = '3';


class TaskInstance {
public:
    TaskInstance();
    TaskInstance(const TaskInstance &old);

    short movesCount; // number of moves made by queen
    short blacksCount; // sum of present black peons

    pair<short, short> queenPosition;
    char board[ARR_INIT_SIZE][ARR_INIT_SIZE];

    void readFromFile(const string &filename, short &k, short &h);
    vector<pair<short, short>> getPossibleMoves(short const &k);

    void printTaskInfo(short &k, short &h);
    void printBoard(short &k);

private:
    bool shouldMoveFurther(short const &x, short const &y, vector<pair<short, short>> &possibleMoves);
};


#endif //CV1_BOARD_H
