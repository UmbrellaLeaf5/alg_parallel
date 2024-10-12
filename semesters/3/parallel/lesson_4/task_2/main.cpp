#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "parallel.hpp"
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

int main(int argc, char* argv[]) {
  parallel::Init(argc, argv);

  int ranks_amount = parallel::RanksAmount();
  int curr_rank = parallel::CurrRank();

  int N;
  double zero_part_of_pi = 0;

  if (curr_rank == 0) {
    N = NumberFromFile("N.dat");

    for (int i = 1; i < ranks_amount; i++) {
      parallel::Send(N, MPI_INT, i);

      std::cout << "Rank: " << curr_rank << ";   Sended: " << N
                << ";   To: " << i << std::endl;
    }
  }

  if (curr_rank != 0) {
    MPI_Status status;

    parallel::Receive(N, MPI_INT, status, 0);

    std::cout << "Rank: " << curr_rank << "; Received: " << N
              << "; From: " << status.MPI_SOURCE << std::endl;

    double part_of_pi = PartOfPi(N, curr_rank * (N / ranks_amount),
                                 (curr_rank + 1) * (N / ranks_amount));
    parallel::Send(part_of_pi, MPI_DOUBLE, 0);

    std::cout << "Rank: " << curr_rank << ";   Sended: " << part_of_pi
              << ";   To: " << 0 << std::endl;

  } else
    zero_part_of_pi = PartOfPi(N, curr_rank * (N / ranks_amount),
                               (curr_rank + 1) * (N / ranks_amount));

  double pi = 0;

  if (curr_rank == 0) {
    for (int i = 1; i < ranks_amount; i++) {
      MPI_Status status;

      double part_of_pi;
      parallel::Receive(part_of_pi, MPI_DOUBLE, status);

      std::cout << "Rank: " << curr_rank << "; Received: " << part_of_pi
                << "; From: " << status.MPI_SOURCE << std::endl;

      pi += part_of_pi;
    }

    pi += zero_part_of_pi;
    std::cout << "Pi: " << pi << std::endl;
  }

  MPI_Finalize();
}
