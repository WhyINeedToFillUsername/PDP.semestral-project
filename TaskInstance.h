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


static const int EMPTY_SQUARE = 0;
static const int BLACK_PEON = 1;
static const int WHITE_PEON = 2;
static const int QUEEN = 3;

class TaskInstance {
public:
    TaskInstance();

    int k; // rozměr šachovnice
    int h; // doporučená hodnota horní meze (akt_min)

    int moves; // number of moves made by queen
    int blacksCount; // sum of present black peons

    pair<int, int> queen;

    void readFromFile(const char *filename);

    void printInfo();
    void printBoard();
    void printPossibleMoves(vector<pair<int, int>> moves);

    vector<pair<int, int>> getPossibleMoves();
    vector<vector<int>> board;

private:
};


#endif //CV1_BOARD_H
