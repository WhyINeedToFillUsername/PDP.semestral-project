#include "mpi.h"
#include <iostream>
#include <queue>
#include <omp.h>
#include "TaskInstance.h"

using namespace std;

#define THRESHOLD 4
#define LOCAL_AMOUNT 50

// MPI tags
#define BEST_SOL_TAG 1
#define END_TAG 4
#define RESULT_TAG 5
#define DATA_TAG 6
#define COUNT_TAG 7
#define GIMME_WORK_TAG 8
#define LENGTH 500

void generateStatesRecursively(TaskInstance task, int x, int y, int treeLevel);
void solveStatesRecursively(TaskInstance *oldTask);

void printMoves(vector<pair<int, int>> &moves);

void sendState(const TaskInstance &state, char *buffer, int &slave);
TaskInstance *receiveState(char * buffer);


int k; // chessboard size
int h; // recommended value of the upper bound (akt_min)
int localBestSolution;
int globalBestSolution;
vector<pair<int, int>> bestSolutionMoves;
queue<TaskInstance> statesToDo;

// compile with "g++ -O3 -fopenmp main.cpp TaskInstance.cpp TaskInstance.h -o cv1.exe"
int main(int argc, char **argv) {

    /* start up MPI */
    MPI_Init(&argc, &argv);

    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int p; // number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int end = false;
    char buffer[LENGTH];
    int position = 0;

    MPI_Status status;
    double startTime;

    if (my_rank == 0) { // master
        const string filename = argv[1];
        cout << filename << endl;

        TaskInstance initTask;
        initTask.readFromFile("./input/" + filename, k, h);
        initTask.printTaskInfo(k, h);

        localBestSolution = h + 1; // set upper bound (h from file input); +1 for the initial position
        globalBestSolution = localBestSolution;

        // measure execution time
        startTime = omp_get_wtime();

        // create states
        statesToDo = queue<TaskInstance>();
        generateStatesRecursively(initTask, initTask.queenPosition[0], initTask.queenPosition[1], 1);
//        cout << "____________# of states: " << statesToDo.size() << endl;

        // send init info
        MPI_Bcast(&localBestSolution, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&h, 1, MPI_INT, 0, MPI_COMM_WORLD);

        while (!statesToDo.empty()) {
            MPI_Recv(&localBestSolution, 1, MPI_INT, MPI_ANY_SOURCE, GIMME_WORK_TAG, MPI_COMM_WORLD, &status);
//            if (localBestSolution < globalBestSolution) globalBestSolution = localBestSolution;
            int receiver = status.MPI_SOURCE;

            int expectStates = min(LOCAL_AMOUNT, static_cast<const int &>(statesToDo.size())); // 10 or remaining
            MPI_Send(&expectStates, 1, MPI_INT, receiver, COUNT_TAG, MPI_COMM_WORLD);
//            cout << "sending " << expectStates << " states to " << receiver << endl;
            for (int j = 0; j < expectStates; j++) {
                sendState(statesToDo.front(), buffer, receiver);
                statesToDo.pop();
            }
        }

        // posli Slavum ukonceni
        cout << endl << "FINISH ALL" << endl;
        end = true;
        for (int i = 1; i < p; i++) {
            MPI_Send(&end, 1, MPI_INT, i, END_TAG, MPI_COMM_WORLD);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    } else { // slave
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        MPI_Bcast(&localBestSolution, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&h, 1, MPI_INT, 0, MPI_COMM_WORLD);
//        cout << ">> slave " << my_rank << " received: ";
//        cout << "bestSolution " << bestSolution << ", k: " << k << ", h: " << h << endl;

        // request first batch of work
        MPI_Send(&localBestSolution, 1, MPI_INT, 0, GIMME_WORK_TAG, MPI_COMM_WORLD);

        // while not received end_tag, work
        while (!end) {
            int expectStates;
            MPI_Recv(&expectStates, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status); //TODO receive best solution so far

            if (status.MPI_TAG == END_TAG) { // end this
                end = expectStates;
            } else if (status.MPI_TAG == COUNT_TAG) { // receive work
//                cout << ">> slave " << my_rank << " is expecting " << expectStates << " states" << endl;

                vector<TaskInstance *> statesToDoInSlave = vector<TaskInstance *>();
                for (int i = 0; i < expectStates; i++) {// for expectStates receive data
                    statesToDoInSlave.push_back(receiveState(buffer));
                }
//                cout << ">> slave " << my_rank << " received " << statesToDoInSlave.size() << " states; solving..." << endl;

                # pragma omp parallel
                {
                    # pragma omp single
                    for (int i = 0; i < expectStates; i++) {// for expectStates receive data
                        TaskInstance *task = statesToDoInSlave[i];
                        #pragma omp task
                        solveStatesRecursively(task);
                    }
                };
            }
            // request work
            MPI_Send(&localBestSolution, 1, MPI_INT, 0, GIMME_WORK_TAG, MPI_COMM_WORLD);
        }
        cout << ">> slave " << my_rank << " received end_tag, closing with best " << localBestSolution << endl << endl;
    }

    int local[2];
    if (my_rank == 0) local[0] = globalBestSolution; else local[0] = localBestSolution;
    local[1] = my_rank;
    int best[2];
    MPI_Allreduce(&local, &best, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD); //implicit barrier

    if (my_rank == 0) {
        const double finalTime = omp_get_wtime() - startTime;
        cout << endl << "total time: " << finalTime << endl;

    } else if (my_rank == best[1]) { // this process has the best solution, print it
        cout << "bestSolution: " << (localBestSolution - 1) << endl; // -1 for the first queen position
        printMoves(bestSolutionMoves);
    }

    /* shut down MPI */
    MPI_Finalize();

    return 0;
}

TaskInstance *receiveState(char *buffer) {
    int position = 0;
    int blacksCount;
    int queenPosition[2];
    char board[ARR_INIT_SIZE][ARR_INIT_SIZE];
    int movesCount;
    MPI_Status status;

    MPI_Recv(buffer, LENGTH, MPI_PACKED, 0, DATA_TAG, MPI_COMM_WORLD, &status);
    MPI_Unpack(buffer, LENGTH, &position, &blacksCount, 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Unpack(buffer, LENGTH, &position, &queenPosition, 2, MPI_INT, MPI_COMM_WORLD);
    MPI_Unpack(buffer, LENGTH, &position, &board, ARR_INIT_SIZE * ARR_INIT_SIZE, MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, LENGTH, &position, &movesCount, 1, MPI_INT, MPI_COMM_WORLD);
    int moves[movesCount][2];
    MPI_Unpack(buffer, LENGTH, &position, &moves, movesCount * 2, MPI_INT, MPI_COMM_WORLD);

    return new TaskInstance(movesCount, blacksCount, queenPosition, *board, moves);
}

void sendState(const TaskInstance &state, char *buffer, int &slave) {
//    printf("sending data to slaves: %d %d %d\n", state.blacksCount, state.queenPosition[0], state.queenPosition[1]);

    int position = 0;
    MPI_Pack(&state.blacksCount, 1, MPI_INT, buffer, LENGTH, &position, MPI_COMM_WORLD);
    MPI_Pack(&state.queenPosition, 2, MPI_INT, buffer, LENGTH, &position, MPI_COMM_WORLD);
    MPI_Pack(&state.board, ARR_INIT_SIZE * ARR_INIT_SIZE, MPI_CHAR, buffer, LENGTH, &position, MPI_COMM_WORLD);

    // pack moves
    int moves[state.movesCount][2];
    for (int i = 0; i < state.movesCount; i++) {
        moves[i][0] = state.madeMoves[i].first;
        moves[i][1] = state.madeMoves[i].second;
    }
    MPI_Pack(&state.movesCount, 1, MPI_INT, buffer, LENGTH, &position, MPI_COMM_WORLD);
    MPI_Pack(&moves, state.movesCount * 2, MPI_INT, buffer, LENGTH, &position, MPI_COMM_WORLD);

    MPI_Send(buffer, position, MPI_PACKED, slave, DATA_TAG, MPI_COMM_WORLD);
}

void generateStatesRecursively(TaskInstance task, int x, int y, int treeLevel) {
    task.movesCount++;
    task.madeMoves.emplace_back(x, y); // record the queen movement

    if (task.board[x][y] == BLACK_PEON) {
        task.blacksCount--;
    }

    // this can't be better than our best solution, don't continue
    if (task.movesCount + task.blacksCount >= localBestSolution) return;

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
        statesToDo.push(task);
        return;
    }
}

void solveStatesRecursively(TaskInstance *oldTask) {
    vector<pair<int, int>> possibleMoves = vector<pair<int, int>>();
    oldTask->getPossibleMoves(k, possibleMoves);

    for (auto &newPosition : possibleMoves) {
        TaskInstance task = *oldTask;
        task.movesCount++;
        task.madeMoves.push_back(newPosition); // record the queen movement

        if (task.board[newPosition.first][newPosition.second] == BLACK_PEON) {
            task.blacksCount--;

            if (task.blacksCount <= 0) {
                // We've eliminated all the black peons. Is this the best solution?
                if (task.movesCount < localBestSolution) {
                    #pragma omp critical
                    {
                        if (task.movesCount < localBestSolution) {
                            localBestSolution = task.movesCount;
                            bestSolutionMoves = task.madeMoves;
                        }
                    };
                }
                break;
            }
        }

        // this can't be better than our best solution, don't continue
        if (task.movesCount + task.blacksCount >= localBestSolution) break;

        // update the queen's position ont the board
        task.board[task.queenPosition[0]][task.queenPosition[1]] = EMPTY_SQUARE;
        task.queenPosition[0] = newPosition.first;
        task.queenPosition[1] = newPosition.second;
        task.board[newPosition.first][newPosition.second] = QUEEN;

        solveStatesRecursively(&task);
    }
}

void printMoves(vector<pair<int, int>> &moves) {
    cout << "queen moves: ";
    for (auto &move : moves) {
        cout << "(" << move.first << "," << move.second << "); ";
    }
    cout << endl;
}
