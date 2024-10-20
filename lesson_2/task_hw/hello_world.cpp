#include <mpi.h>

#include <iostream>

int main(int argc, char *argv[]) {
  int st;
  st = MPI_Init(&argc, &argv);
  if (st != MPI_SUCCESS) {
    MPI_Abort(MPI_COMM_WORLD, st);
  }
  std::cout << "Hello World!" << std::endl;
  MPI_Finalize();
}
