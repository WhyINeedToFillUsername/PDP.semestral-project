//
// Created by akarola on 012 12. 3. 18.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#ifndef CV1_BOARD_H
#define CV1_BOARD_H


static const short EMPTY_SQUARE = 0;
static const short BLACK_PEON = 1;
static const short WHITE_PEON = 2;
static const short QUEEN = 3;

class TaskInstance {
public:
    TaskInstance();
    TaskInstance(const TaskInstance &old);

    short k; // rozměr šachovnice
    short h; // doporučená hodnota horní meze (akt_min)

    short movesCount; // number of moves made by queen
    short blacksCount; // sum of present black peons

    pair<short, short> queenPosition;
    vector<vector<short>> board;

    void readFromFile(const string &filename);
    vector<pair<short, short>> getPossibleMoves();

    void printTaskInfo();
    void printBoard();

private:
    bool shouldMoveFurther(short x, short y, vector<pair<short, short>> &possibleMoves);
};


#endif //CV1_BOARD_H
