#include <iostream>
#include "TaskInstance.h"

static const char *const filename = R"(C:\Users\akarola\Dropbox\School\FIT LS 2018\PDP\Semestralka\cv1\input\kralovna01.txt)";
using namespace std;

void recursion(TaskInstance *task);

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
    recursion(task);
    return 0;
}

void recursion(TaskInstance *task) {

    // seřaď je - první ty, co vyhazují
    // rekurzivně zavolej to nad sebou, dokud nezbydou žádné černé figurky

    // zjisti všechny možné tahy
}