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

  if (curr_rank == 0) N = NumberFromFile("N.dat");

  parallel::AbortTest(MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD));

  double part_of_pi;

  part_of_pi = PartOfPi(N, curr_rank * (N / ranks_amount),
                        (curr_rank + 1) * (N / ranks_amount));

  double pi = 0;

  parallel::AbortTest(
      MPI_Reduce(&part_of_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD));

  if (curr_rank == 0) std::cout << "Pi: " << pi << std::endl;

  parallel::Finalize();
}
