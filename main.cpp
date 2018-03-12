#include <iostream>
#include "TaskInstance.h"

static const char *const filename = R"(C:\Users\akarola\Dropbox\School\FIT LS 2018\PDP\Semestralka\cv1\input\kralovna01.txt)";
using namespace std;

void recursion(TaskInstance *task, pair<int, int> queenPosition);

int bestSolution = INT_MAX;

int main(int argc, char **argv) {
    TaskInstance *task;
    task = new TaskInstance;
    task->readFromFile(filename);
    task->printInfo();
    task->printBoard();

    // zjisti všechny možné tahy
    vector<pair<int, int>> possibleMoves = task->getPossibleMoves();
    task->printPossibleMoves(possibleMoves);

    // for every move call:
    for (auto &&newPosition : possibleMoves) {
        recursion(task, newPosition);
    }
    return 0;
}

void recursion(TaskInstance *task, pair<int, int> queenPosition) {

    // rekurzivně zavolej to nad sebou, dokud nezbydou žádné černé figurky
    // zjisti všechny možné tahy

    task->moves++;
    if (task->board[queenPosition.first][queenPosition.second] == BLACK_PEON) {
        task->board[queenPosition.first][queenPosition.second] = EMPTY_SQUARE;
        task->blacksCount--;

        if (task->blacksCount <= 0) {
            // We've eliminated all the black peons. Is this the best solution?
            if (task->moves < bestSolution) bestSolution = task->moves;
            return;
        }
    }

    if (task->moves >= bestSolution) return; // this will be worse then our best solution, don't continue
    for (auto &&newPosition : task->getPossibleMoves()) {
        recursion(task, newPosition);
    }
}