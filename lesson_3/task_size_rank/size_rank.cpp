#include <mpi.h>

#include <iostream>

void CheckSuccess(int state) {
  if (state != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, state);
}

int main(int argc, char *argv[]) {
  int st;
  st = MPI_Init(&argc, &argv);
  CheckSuccess(st);

  int size = 0;
  st = MPI_Comm_size(MPI_COMM_WORLD, &size);
  CheckSuccess(st);

  int rank = 0;
  st = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  CheckSuccess(st);

  std::cout << "Hello World! "
            << "We are processes, how many: " << size << "; our rank: " << rank
            << std::endl;

  MPI_Finalize();
}
