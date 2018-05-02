#include <iostream>
#include <omp.h>
#include "TaskInstance.h"

using namespace std;

void recursion(TaskInstance task, pair<short, short> queenNewPosition, vector<pair<short, short>> moves);

void printMoves(vector<pair<short, short>> moves);

short bestSolution;
vector<pair<short, short>> madeMoves;

int main(int argc, char **argv) {
    const string filename = argv[1];
    cout << filename << endl;

    TaskInstance task;
    task.readFromFile("./input/" + filename);
    task.printTaskInfo();

    bestSolution = task.h; // set upper bound (h from file input)

    // measure execution time
    double startTime = omp_get_wtime();

    vector<pair<short, short>> possibleMoves = task.getPossibleMoves();
//    printMoves(possibleMoves);
    cout << endl;

    vector<pair<short, short>> moves = vector<pair<short, short>>();
    moves.push_back(task.queenPosition);

    // for every move call:
    for (auto &newPosition : possibleMoves) {
//        cout << "newPosition: " << "(" << newPosition.first << "," << newPosition.second << ")" << endl;
        recursion(task, newPosition, moves);
    }

    const double finalTime = omp_get_wtime() - startTime;
    cout << endl << "total time: " << finalTime << endl;

    cout << endl << "bestSolution: " << bestSolution << endl;

    printMoves(madeMoves);

    return 0;
}

void recursion(TaskInstance task, pair<short, short> queenNewPosition, vector<pair<short, short>> moves) {
    task.movesCount++;

    moves.push_back(queenNewPosition); // record the queen movement

    if (task.board[queenNewPosition.first][queenNewPosition.second] == BLACK_PEON) {
//        task.board[queenNewPosition.first][queenNewPosition.second] = EMPTY_SQUARE;
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

    // update the queen's position ontthe board
    task.board[task.queenPosition.first][task.queenPosition.second] = EMPTY_SQUARE;
    task.queenPosition.first = queenNewPosition.first;
    task.queenPosition.second = queenNewPosition.second;
    task.board[queenNewPosition.first][queenNewPosition.second] = QUEEN;

//    task.printTaskInfo();
    vector<pair<short, short>> possibleMoves = task.getPossibleMoves();
//    printMoves(possibleMoves);
    for (auto &newPosition : possibleMoves) {
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
