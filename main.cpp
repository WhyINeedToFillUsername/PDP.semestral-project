#include "mpi.h"
#include <iostream>
#include "TaskInstance.h"

using namespace std;

#define THRESHOLD 4

// MPI tags
#define BEST_SOL_TAG 1
#define K_TAG 2
#define H_TAG 3
#define END_TAG 4
#define RESULT_TAG 5
#define DATA_TAG 6
#define LENGTH 100

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
    char buffer[LENGTH];
    int position = 0;

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
        generateStatesRecursively(initTask, initTask.queenPosition[0], initTask.queenPosition[1], 1);

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

        cout << "sizeof(statesToDo[0].movesCount): " << sizeof(statesToDo[0].madeMoves.size()) << endl;
        cout << "sizeof(statesToDo[0]): " << sizeof(statesToDo[0]) << endl;

        //    # pragma omp parallel for
        for (int i = 0; i < statesToDo.size(); i++) {
            vector<pair<int, int>> possibleMoves = vector<pair<int, int>>();
            statesToDo[i].getPossibleMoves(k, possibleMoves);

            for (int j = 0; j < possibleMoves.size(); j++) {
                solveStatesRecursively(statesToDo[i], possibleMoves[j]);
            }
        }

        /* send:
        int blacksCount; // sum of present black peons
        int queenPosition[2];
        char board[ARR_INIT_SIZE][ARR_INIT_SIZE];
        vector<pair<int, int>> madeMoves;*/

//        MPI_Send(&data.front(), data.size(), MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
//        &chars[0], chars.size()

//        MPI_Pack(&statesToDo[0].blacksCount, 1, MPI_INT, buffer, LENGTH, &position, MPI_COMM_WORLD);
//        MPI_Pack(&statesToDo[0].queenPosition, 2, MPI_INT, buffer, LENGTH, &position, MPI_COMM_WORLD);
//        dest = 0;
//        MPI_Send(buffer, position, MPI_PACKED, dest, DATA_TAG, MPI_COMM_WORLD);

        // posli Slavum ukonceni
        cout << endl << "FINISH ALL" << endl;
        end = true;
        for (int i = 1; i < p; i++) {
            MPI_Send(&end, 1, MPI_SHORT, i, END_TAG, MPI_COMM_WORLD);
        }

        // prijmi od Slavu vysledek
        cout << "slave results: ";
        int slaveResults[p - 1];
        for (int i = 1; i < p; i++) {
            MPI_Recv(&slaveResults[i - 1], 1, MPI_INT, i, RESULT_TAG, MPI_COMM_WORLD, &status);
            cout << i << ": " << slaveResults[i - 1] << "; ";
        }
        cout << endl;

//        MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
        const double finalTime = MPI_Wtime() - startTime;
        cout << endl << "total time: " << finalTime << endl;

        // -1 for the first queen position
        cout << "bestSolution: " << (bestSolution - 1) << endl;

        printMoves(bestSolutionMoves);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    } else { // slave
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        MPI_Recv(&bestSolution, 1, MPI_INT, 0, BEST_SOL_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&k, 1, MPI_INT, 0, K_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&h, 1, MPI_INT, 0, H_TAG, MPI_COMM_WORLD, &status);
        cout << ">> slave " << my_rank << " received: ";
        cout << "bestSolution " << bestSolution << ", k: " << k << ", h: " << h << endl;

        int myBest = bestSolution;

//        MPI_Recv(buffer, LENGTH, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//        MPI_Unpack(buffer, LENGTH, &position, &a, 1, MPI_FLOAT, MPI_COMM_WORLD);

        // while not received end_tag, work
        while (!end) {
            MPI_Recv(&end, 1, MPI_SHORT, 0, END_TAG, MPI_COMM_WORLD, &status);
        }

        // send my best solution
        MPI_Send(&myBest, 1, MPI_INT, 0, RESULT_TAG, MPI_COMM_WORLD);

        cout << ">> slave " << my_rank << " received end_tag, closing." << endl;
    }

    /* shut down MPI */
    MPI_Finalize();

    return 0;
}

void generateStatesRecursively(TaskInstance task, int x, int y, int treeLevel) {
    task.madeMoves.emplace_back(x, y); // record the queen movement
    int movesCount = static_cast<int>(task.madeMoves.size());

    if (task.board[x][y] == BLACK_PEON) {
        task.blacksCount--;
    }

    // this can't be better than our best solution, don't continue
    if (movesCount + task.blacksCount >= bestSolution) return;

    // update the queen's position ont the board
    task.board[task.queenPosition[0]][task.queenPosition[1]] = EMPTY_SQUARE;
    task.queenPosition[0] = x;
    task.queenPosition[1] = y;
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
    task.madeMoves.push_back(queenNewPosition); // record the queen movement
    int movesCount = static_cast<int>(task.madeMoves.size());

    if (task.board[queenNewPosition.first][queenNewPosition.second] == BLACK_PEON) {
        task.blacksCount--;

        if (task.blacksCount <= 0) {
            // We've eliminated all the black peons. Is this the best solution?
            if (movesCount < bestSolution) {
//                #pragma omp critical
                {
                    if (movesCount < bestSolution) {
                        bestSolution = movesCount;
                        bestSolutionMoves = task.madeMoves;
                    }
                };
            }
            return;
        }
    }

    // this can't be better than our best solution, don't continue
    if (movesCount + task.blacksCount >= bestSolution) return;

    // update the queen's position ont the board
    task.board[task.queenPosition[0]][task.queenPosition[1]] = EMPTY_SQUARE;
    task.queenPosition[0] = queenNewPosition.first;
    task.queenPosition[1] = queenNewPosition.second;
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
