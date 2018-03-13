#include <iostream>
#include "TaskInstance.h"

static const char *const filename = R"(c:\Users\akarola\Dropbox\School\FIT LS 2018\PDP\Semestralka\pdp_kralovna\kralovna01.txt)";
using namespace std;

void recursion(TaskInstance task, pair<int, int> queenPosition, vector<pair<int, int>> moves);

void printMoves(vector<pair<int, int>> moves);

int bestSolution = INT_MAX;
vector<pair<int, int>> madeMoves;

int main(int argc, char **argv) {
    TaskInstance task;
//    task = new TaskInstance;
    task.readFromFile(filename);
    task.printInfo();
    task.printBoard();

    // zjisti všechny možné tahy
    vector<pair<int, int>> possibleMoves = task.getPossibleMoves();
    printMoves(possibleMoves);
    cout << endl;

    vector<pair<int, int>> moves = vector<pair<int, int>>();
    moves.push_back(task.queenPosition);

    // for every move call:
    for (auto &&newPosition : possibleMoves) {
        recursion(task, newPosition, moves);
    }

    cout << endl;
    cout << "bestSolution " << bestSolution << endl;

    printMoves(madeMoves);

    return 0;
}

void recursion(TaskInstance task, pair<int, int> queenPosition, vector<pair<int, int>> moves) {
//    task.printInfo();
//    task.printBoard();
//    cout << endl;

    // rekurzivně zavolej to nad sebou, dokud nezbydou žádné černé figurky
    // zjisti všechny možné tahy

    task.movesCount++;
    moves.push_back(queenPosition);

    if (task.board[queenPosition.first][queenPosition.second] == BLACK_PEON) {
        task.board[queenPosition.first][queenPosition.second] = EMPTY_SQUARE;
        task.blacksCount--;

        if (task.blacksCount <= 0) {
            // We've eliminated all the black peons. Is this the best solution?
            if (task.movesCount < bestSolution) bestSolution = task.movesCount;
            madeMoves = moves;
            return;
        }
    }

    if (task.movesCount >= bestSolution) return; // this will be worse then our best solution, don't continue
    for (auto &&newPosition : task.getPossibleMoves()) {
        recursion(task, newPosition, moves);
    }
}

void printMoves(vector<pair<int, int>> moves) {
    cout << "queen moves: ";
    for (auto &&item : moves) {
        cout << "(" << item.first << "," << item.second << "); ";
    }
    cout << endl;
}
