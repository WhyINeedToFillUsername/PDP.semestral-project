#include "mpi.h"
#include <iostream>
#include "TaskInstance.h"

using namespace std;

const int THRESHOLD = 4;

// MPI tags
const int BEST_SOL_TAG = 1;
const int K_TAG = 2;
const int H_TAG = 3;
const int END_TAG = 4;
const int RESULT_TAG = 5;

void generateStatesRecursively(TaskInstance task, int x, int y, int treeLevel);
void solveStatesRecursively(TaskInstance task, pair<int, int> queenNewPosition);

void printMoves(vector<pair<int, int>> &moves);

int k; // chessboard size
int h; // recommended value of the upper bound (akt_min)
int bestSolution;
vector<pair<int, int>> bestSolutionMoves;
vector<TaskInstance> statesToDo;

// compile with "g++ -O3 -fopenmp main.cpp TaskInstance.cpp TaskInstance.h -o cv1.exe"
int main(int argc, char **argv) {

    /* start up MPI */
    MPI_Init(&argc, &argv);

    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int p; // number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    short end = false;

    MPI_Status status;

    if (my_rank == 0) { // master
        const string filename = argv[1];
        cout << filename << endl;

        TaskInstance initTask;
        initTask.readFromFile("./input/" + filename, k, h);
        initTask.printTaskInfo(k, h);

        bestSolution = h + 1; // set upper bound (h from file input); +1 for the initial position

        // measure execution time
        double startTime = MPI_Wtime();

        // create states
        statesToDo = vector<TaskInstance>();
        generateStatesRecursively(initTask, initTask.queenPosition.first, initTask.queenPosition.second, 1);

        // TODO states send to slaves

        // send init info
        for (int i = 1; i < p; i++) {
            MPI_Send(&bestSolution, 1, MPI_INT, i, BEST_SOL_TAG, MPI_COMM_WORLD);
            MPI_Send(&k, 1, MPI_INT, i, K_TAG, MPI_COMM_WORLD);
            MPI_Send(&h, 1, MPI_INT, i, H_TAG, MPI_COMM_WORLD);
        }

//        while (!QUEUE.Empty()) {
//            // send
//        }

        // posli Slavum ukonceni
        cout << endl << "FINISH ALL" << endl;
        end = true;
        for (int i = 1; i < p; i++) {
            MPI_Send(&end, 1, MPI_SHORT, i, END_TAG, MPI_COMM_WORLD);
        }
        //prijmi od Slavu vysledek
//        for (int i = 0; i < Slave.Count(); i++) MPI_Recv(vysledek, ...);

//    # pragma omp parallel for
//    for (int i = 0; i < statesToDo.size(); i++) {
//        vector<pair<int, int>> possibleMoves = vector<pair<int, int>>();
//        statesToDo[i].getPossibleMoves(k, possibleMoves);
//
//        for (int j = 0; j < possibleMoves.size(); j++) {
//            solveStatesRecursively(statesToDo[i], possibleMoves[j]);
//        }
//    }

//        MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
        const double finalTime = MPI_Wtime() - startTime;
        cout << endl << "total time: " << finalTime << endl;

        // -1 for the first queen position
        cout << "bestSolution: " << (bestSolution - 1) << endl;

        printMoves(bestSolutionMoves);

    } else { // slave
        MPI_Recv(&bestSolution, 1, MPI_INT, 0, BEST_SOL_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&k, 1, MPI_INT, 0, K_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&h, 1, MPI_INT, 0, H_TAG, MPI_COMM_WORLD, &status);
        cout << ">> slave " << my_rank << " received: ";
        cout << "bestSolution " << bestSolution << ", k: " << k << ", h: " << h << endl;

        while (!end) {
            MPI_Recv(&end, 1, MPI_SHORT, 0, END_TAG, MPI_COMM_WORLD, &status);
        }

        cout << ">> slave " << my_rank << " received end_tag, closing." << endl;
    }

    /* shut down MPI */
    MPI_Finalize();

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

void solveStatesRecursively(TaskInstance task, pair<int, int> queenNewPosition) {
    task.movesCount++;
    task.madeMoves.push_back(queenNewPosition); // record the queen movement

    if (task.board[queenNewPosition.first][queenNewPosition.second] == BLACK_PEON) {
        task.blacksCount--;

        if (task.blacksCount <= 0) {
            // We've eliminated all the black peons. Is this the best solution?
            if (task.movesCount < bestSolution) {
//                #pragma omp critical
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
        solveStatesRecursively(task, newPosition);
    }
}

void printMoves(vector<pair<int, int>> &moves) {
    cout << "queen moves: ";
    for (auto &move : moves) {
        cout << "(" << move.first << "," << move.second << "); ";
    }
    cout << endl;
}
