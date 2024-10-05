#include <mpi.h>

#include <iostream>

void AbortTest(int state) {
  if (state != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, state);
}

int main(int argc, char *argv[]) {
  int st;
  st = MPI_Init(&argc, &argv);
  AbortTest(st);

  MPI_Finalize();
}
