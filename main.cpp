#include <iostream>
#include "TaskInstance.h"

using namespace std;

void recursion(TaskInstance task, pair<short, short> queenNewPosition, vector<pair<short, short>> moves);

void printMoves(vector<pair<short, short>> moves);

short k; // rozměr šachovnice
short h; // doporučená hodnota horní meze (akt_min)
short bestSolution;
vector<pair<short, short>> madeMoves;

// compile with "g++ -O3 -fopenmp main.cpp TaskInstance.cpp TaskInstance.h -o cv1.exe"
int main(int argc, char **argv) {
    const string filename = argv[1];
    cout << filename << endl;

    TaskInstance task;
    task.readFromFile("./input/" + filename, k, h);
    task.printTaskInfo(k, h);

    bestSolution = h; // set upper bound (h from file input)

    vector<pair<short, short>> possibleMoves = task.getPossibleMoves(k);
    printMoves(possibleMoves);
    cout << endl;

    # pragma omp parallel
    # pragma omp single
    recursion(task, task.queenPosition, vector<pair<short, short>>());

    // -1 for the first queen position
    cout << endl << "bestSolution: " << (bestSolution - 1) << endl;

    printMoves(madeMoves);

    return 0;
}

void recursion(TaskInstance task, pair<short, short> queenNewPosition, vector<pair<short, short>> moves) {
    task.movesCount++;
    moves.push_back(queenNewPosition); // record the queen movement

    if (task.board[queenNewPosition.first][queenNewPosition.second] == BLACK_PEON) {
        task.blacksCount--;

        if (task.blacksCount <= 0) {
            // We've eliminated all the black peons. Is this the best solution?
            if (task.movesCount < bestSolution) {
                bestSolution = task.movesCount;
                madeMoves = moves;
            }
            return;
        }
    }

    // this can't be better than our best solution, don't continue
    if (task.movesCount + task.blacksCount >= bestSolution) return;

    // update the queen's position ont the board
    task.board[task.queenPosition.first][task.queenPosition.second] = EMPTY_SQUARE;
    task.queenPosition.first = queenNewPosition.first;
    task.queenPosition.second = queenNewPosition.second;
    task.board[queenNewPosition.first][queenNewPosition.second] = QUEEN;

    for (auto &newPosition : task.getPossibleMoves(k)) {
        #pragma omp task
        recursion(task, newPosition, moves);
    }
}

void printMoves(vector<pair<short, short>> moves) {
    cout << "queen moves: ";
    for (auto &move : moves) {
        cout << "(" << move.first << "," << move.second << "); ";
    }
    cout << endl;
}
