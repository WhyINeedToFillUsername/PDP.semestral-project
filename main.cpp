#include <iostream>
#include "TaskInstance.h"

static const char *const filename = R"(c:\Users\akarola\Dropbox\School\FIT LS 2018\PDP\Semestralka\pdp_kralovna\kralovna04.txt)";
using namespace std;

void recursion(TaskInstance task, pair<int, int> queenNewPosition, vector<pair<int, int>> moves);

void printMoves(vector<pair<int, int>> moves);

int bestSolution;
vector<pair<int, int>> madeMoves;

int main(int argc, char **argv) {
    TaskInstance task;
    task.readFromFile(filename);
    task.printTaskInfo();

    bestSolution = task.h; // set upper bound (h from file input)

    vector<pair<int, int>> possibleMoves = task.getPossibleMoves();
    printMoves(possibleMoves);
    cout << endl;

    vector<pair<int, int>> moves = vector<pair<int, int>>();
    moves.push_back(task.queenPosition);

    // for every move call:
    for (auto &newPosition : possibleMoves) {
//        cout << "newPosition: " << "(" << newPosition.first << "," << newPosition.second << ")" << endl;
        recursion(task, newPosition, moves);
    }

    cout << endl << "bestSolution: " << bestSolution << endl;

    printMoves(madeMoves);

    return 0;
}

void recursion(TaskInstance task, pair<int, int> queenNewPosition, vector<pair<int, int>> moves) {
    task.movesCount++;

    // remove the queen's old position from the board
    task.board[task.queenPosition.first][task.queenPosition.second] = EMPTY_SQUARE;
    task.queenPosition = queenNewPosition;

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

    // put the queen's new position on the board
    task.board[queenNewPosition.first][queenNewPosition.second] = QUEEN;

    for (auto &newPosition : task.getPossibleMoves()) {
        recursion(task, newPosition, moves);
    }
}

void printMoves(vector<pair<int, int>> moves) {
    cout << "queen moves: ";
    for (auto &move : moves) {
        cout << "(" << move.first << "," << move.second << "); ";
    }
    cout << endl;
}
