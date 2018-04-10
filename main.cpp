#include <iostream>
#include <omp.h>
#include "TaskInstance.h"

using namespace std;

const int THRESHOLD = 4;

void generateStatesRecursively(TaskInstance task, int x, int y, int treeLevel);
void recursionSequential(TaskInstance task, pair<int, int> queenNewPosition);

void printMoves(vector<pair<int, int>> &moves);

int k; // chessboard size
int h; // recommended value of the upper bound (akt_min)
int bestSolution;
vector<pair<int, int>> bestSolutionMoves;
vector<TaskInstance> statesToDo;

// compile with "g++ -O3 -fopenmp main.cpp TaskInstance.cpp TaskInstance.h -o cv1.exe"
int main(int argc, char **argv) {
    const string filename = argv[1];
    cout << filename << endl;

    TaskInstance initTask;
    initTask.readFromFile("./input/" + filename, k, h);
    initTask.printTaskInfo(k, h);

    bestSolution = h + 1; // set upper bound (h from file input); +1 for the initial position

    // measure execution time
    double startTime = omp_get_wtime();

    // create states
    statesToDo = vector<TaskInstance>();
    generateStatesRecursively(initTask, initTask.queenPosition.first, initTask.queenPosition.second, 1);

    # pragma omp parallel for
    for (int i = 0; i < statesToDo.size(); i++) {
        vector<pair<int, int>> possibleMoves = vector<pair<int, int>>();
        statesToDo[i].getPossibleMoves(k, possibleMoves);

        for (int j = 0; j < possibleMoves.size(); j++) {
            recursionSequential(statesToDo[i], possibleMoves[j]);
        }
    }

    const double finalTime = omp_get_wtime() - startTime;
    cout << endl << "total time: " << finalTime << endl;

    // -1 for the first queen position
    cout << "bestSolution: " << (bestSolution - 1) << endl;

    printMoves(bestSolutionMoves);

    return 0;
}

void generateStatesRecursively(TaskInstance task, int x, int y, int treeLevel) {
    task.movesCount++;
    task.madeMoves.emplace_back(x, y); // record the queen movement

    if (task.board[x][y] == BLACK_PEON) {
        task.blacksCount--;
    }

    // this can't be better than our best solution, don't continue
    if (task.movesCount + task.blacksCount >= bestSolution) return;

    // update the queen's position ont the board
    task.board[task.queenPosition.first][task.queenPosition.second] = EMPTY_SQUARE;
    task.queenPosition.first = x;
    task.queenPosition.second = y;
    task.board[x][y] = QUEEN;

    if (treeLevel < THRESHOLD) {
        vector<pair<int, int>> possibleMoves = vector<pair<int, int>>();
        task.getPossibleMoves(k, possibleMoves);

        for (int i = 0; i < possibleMoves.size(); i++) {
            int newX = possibleMoves[i].first;
            int newY = possibleMoves[i].second;
            generateStatesRecursively(task, newX, newY, treeLevel + 1);
        }
    } else {
        statesToDo.push_back(task);
        return;
    }
}

void recursionSequential(TaskInstance task, pair<int, int> queenNewPosition) {
    task.movesCount++;
    task.madeMoves.push_back(queenNewPosition); // record the queen movement

    if (task.board[queenNewPosition.first][queenNewPosition.second] == BLACK_PEON) {
        task.blacksCount--;

        if (task.blacksCount <= 0) {
            // We've eliminated all the black peons. Is this the best solution?
            if (task.movesCount < bestSolution) {
                #pragma omp critical
                {
                    if (task.movesCount < bestSolution) {
                        bestSolution = task.movesCount;
                        bestSolutionMoves = task.madeMoves;
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
        recursionSequential(task, newPosition);
    }
}

void printMoves(vector<pair<int, int>> &moves) {
    cout << "queen moves: ";
    for (auto &move : moves) {
        cout << "(" << move.first << "," << move.second << "); ";
    }
    cout << endl;
}
