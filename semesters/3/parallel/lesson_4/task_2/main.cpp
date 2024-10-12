#include <mpi.h>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "pi.hpp"

/**
 * @brief Считывает число из файла (из первой строки)
 * @param file_name: название файла (по умолчанию "N.dat")
 * @return int: число
 */
int NumberFromFile(const std::string& filename) {
  std::ifstream in(filename.c_str());
  std::string line;

  if (!in.is_open()) {
    throw std::runtime_error("NumberFromFile: file opening error.");
    return 0;
  }

  int number;
  in >> number;

  in.close();

  return number;
}

void AbortTest(int state) {
  if (state != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, state);
}

int main(int argc, char* argv[]) {
  int tag = 35817;
  MPI_Status status;

  int st;
  st = MPI_Init(&argc, &argv);
  AbortTest(st);

  int ranks_amount;
  st = MPI_Comm_size(MPI_COMM_WORLD, &ranks_amount);
  AbortTest(st);

  int curr_rank;
  st = MPI_Comm_rank(MPI_COMM_WORLD, &curr_rank);
  AbortTest(st);

  int N;
  double zero_part_of_pi;

  if (curr_rank == 0) {
    N = NumberFromFile("N.dat");

    for (int i = 1; i < ranks_amount; i++) {
      st = MPI_Send(&N, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
      AbortTest(st);

      std::cout << "Rank: " << curr_rank << ";   Sended: " << N
                << ";   To: " << i << std::endl;
    }
  }

  if (curr_rank != 0) {
    st = MPI_Recv(&N, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
    AbortTest(st);

    std::cout << "Rank: " << curr_rank << "; Received: " << N
              << "; From: " << status.MPI_SOURCE << std::endl;

    double part_of_pi = PartOfPi(N, curr_rank * (N / ranks_amount),
                                 (curr_rank + 1) * (N / ranks_amount));
    st = MPI_Send(&part_of_pi, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
    AbortTest(st);

    std::cout << "Rank: " << curr_rank << ";   Sended: " << part_of_pi
              << ";   To: " << 0 << std::endl;

  } else
    zero_part_of_pi = PartOfPi(N, curr_rank * (N / ranks_amount),
                               (curr_rank + 1) * (N / ranks_amount));

  double pi = 0;

  if (curr_rank == 0) {
    for (int i = 1; i < ranks_amount; i++) {
      double part_of_pi;
      st = MPI_Recv(&part_of_pi, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag,
                    MPI_COMM_WORLD, &status);
      AbortTest(st);

      std::cout << "Rank: " << curr_rank << "; Received: " << part_of_pi
                << "; From: " << status.MPI_SOURCE << std::endl;

      pi += part_of_pi;
    }

    pi += zero_part_of_pi;
    std::cout << "Pi: " << pi << std::endl;
  }

  MPI_Finalize();
}
