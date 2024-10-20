#include <mpi.h>

#include <iostream>

void CheckSuccess(int state) {
  if (state != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, state);
}

int main(int argc, char *argv[]) {
  int st;
  st = MPI_Init(&argc, &argv);
  CheckSuccess(st);

  MPI_Status status;
  int send_data = 17;
  int recv_data;

  int rank;
  st = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  CheckSuccess(st);

  int my_tag = 35817;
  int dest_proc_rank = 1;
  int from_proc_rank = 0;

  if (rank == from_proc_rank) {
    st = MPI_Send(&send_data, 1, MPI_INT, dest_proc_rank, my_tag,
                  MPI_COMM_WORLD);
    CheckSuccess(st);

    std::cout << "Sended  : " << send_data
              << "; from rank: " << rank /*       */
              << "; with tag: " << my_tag << std::endl;
  }

  if (rank == dest_proc_rank) {
    st = MPI_Recv(&recv_data, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
                  MPI_COMM_WORLD, &status);
    CheckSuccess(st);

    std::cout << "Received: " << recv_data
              << ";    source: " << status.MPI_SOURCE
              << "; with tag: " << status.MPI_TAG << std::endl;
  }

  MPI_Finalize();
}
