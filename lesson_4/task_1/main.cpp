#include <mpi.h>

#include <iostream>

void CheckSuccess(int state) {
  if (state != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, state);
}

int main(int argc, char *argv[]) {
  int tag = 35817;
  MPI_Status status;

  int st;
  st = MPI_Init(&argc, &argv);
  CheckSuccess(st);

  int N;
  st = MPI_Comm_size(MPI_COMM_WORLD, &N);
  CheckSuccess(st);

  int rank;
  st = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  CheckSuccess(st);

  int i;
  int dest_rank = 0;  // если dest_rank != N -1, меняется
  if (rank < N && rank != N - 1) dest_rank = rank + 1;

  if (rank == 0) {
    i = 0;
    st = MPI_Send(&i, 1, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);
    CheckSuccess(st);

    std::cout << "Rank: " << rank << ";   Sended: " << i
              << ";   To: " << rank + 1 << std::endl;
  }

  st = MPI_Recv(&i, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
  CheckSuccess(st);

  std::cout << "Rank: " << rank << "; Received: " << i
            << "; From: " << status.MPI_SOURCE << std::endl;

  if (rank != 0) {
    i += rank;
    st = MPI_Send(&i, 1, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);
    CheckSuccess(st);

    std::cout << "Rank: " << rank << ";   Sended: " << i
              << ";   To: " << rank + 1 << std::endl;
  }

  MPI_Finalize();
}
