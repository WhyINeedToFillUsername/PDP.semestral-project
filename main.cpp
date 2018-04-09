#include <iostream>
#include "TaskInstance.h"

using namespace std;

const int THRESHOLD = 5;

void recursionParallel(TaskInstance task, int x, int y, vector<pair<int, int>> moves, int treeLevel);
void recursionSequential(TaskInstance task, pair<int, int> queenNewPosition, vector<pair<int, int>> moves);

void printMoves(vector<pair<int, int>> &moves);

int k; // chessboard size
int h; // recommended value of the upper bound (akt_min)
int bestSolution;
vector<pair<int, int>> madeMoves;

// compile with "g++ -O3 -fopenmp main.cpp TaskInstance.cpp TaskInstance.h -o cv1.exe"
int main(int argc, char **argv) {
    const string filename = argv[1];
    cout << filename << endl;

    TaskInstance task;
    task.readFromFile("./input/" + filename, k, h);
    task.printTaskInfo(k, h);

    bestSolution = h; // set upper bound (h from file input)

    vector<pair<int, int>> possibleMoves = vector<pair<int, int>>();
    task.getPossibleMoves(k, possibleMoves);
    printMoves(possibleMoves);
    cout << endl;

    # pragma omp parallel
    # pragma omp single
    recursionParallel(task, task.queenPosition.first, task.queenPosition.second, vector<pair<int, int>>(), 1);

    // -1 for the first queen position
    cout << endl << "bestSolution: " << (bestSolution - 1) << endl;

    printMoves(madeMoves);

    return 0;
}

void recursionParallel(TaskInstance task, int x, int y, vector<pair<int, int>> moves, int treeLevel) {
    task.movesCount++;
    moves.emplace_back(x, y); // record the queen movement

    if (task.board[x][y] == BLACK_PEON) {
        task.blacksCount--;

        if (task.blacksCount <= 0) {
            // We've eliminated all the black peons. Is this the best solution?
            if (task.movesCount < bestSolution) {
                #pragma omp critical
                {
                    if (task.movesCount < bestSolution) {
                        bestSolution = task.movesCount;
                        madeMoves = moves;
                    }
                };
            }
//            cout << "treeLevel: " << treeLevel << endl;
            return;
        }
    }

    // this can't be better than our best solution, don't continue
    if (task.movesCount + task.blacksCount >= bestSolution) return;

    // update the queen's position ont the board
    task.board[task.queenPosition.first][task.queenPosition.second] = EMPTY_SQUARE;
    task.queenPosition.first = x;
    task.queenPosition.second = y;
    task.board[x][y] = QUEEN;

    vector<pair<int, int>> possibleMoves = vector<pair<int, int>>();
    task.getPossibleMoves(k, possibleMoves);

    for (int i = 0; i < possibleMoves.size() - 1; i++) {
        // limit parallelism with THRESHOLD using "treeLevel", see slides 30 - 32 from 3rd lecture
        if (treeLevel < THRESHOLD) {
            int newX = possibleMoves[i].first;
            int newY = possibleMoves[i].second;
            #pragma omp task shared(newX, newY)
            recursionParallel(task, newX, newY, moves, treeLevel + 1);
        } else {
            recursionSequential(task, possibleMoves[i], moves);
        }
    }
    recursionParallel(task, possibleMoves[possibleMoves.size() - 1].first, possibleMoves[possibleMoves.size() - 1].second, moves, treeLevel + 1);
}

void recursionSequential(TaskInstance task, pair<int, int> queenNewPosition, vector<pair<int, int>> moves) {
    task.movesCount++;
    moves.push_back(queenNewPosition); // record the queen movement

    if (task.board[queenNewPosition.first][queenNewPosition.second] == BLACK_PEON) {
        task.blacksCount--;

        if (task.blacksCount <= 0) {
            // We've eliminated all the black peons. Is this the best solution?
            if (task.movesCount < bestSolution) {
                #pragma omp critical
                {
                    if (task.movesCount < bestSolution) {
                        bestSolution = task.movesCount;
                        madeMoves = moves;
                    }
                };
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

    vector<pair<int, int>> possibleMoves = vector<pair<int, int>>();
    task.getPossibleMoves(k, possibleMoves);

    for (auto &newPosition : possibleMoves) {
        recursionSequential(task, newPosition, moves);
    }
}

void printMoves(vector<pair<int, int>> &moves) {
    cout << "queen moves: ";
    for (auto &move : moves) {
        cout << "(" << move.first << "," << move.second << "); ";
    }
    cout << endl;
}
