#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "parallel.hpp"
#include "utils.hpp"

int main(int argc, char* argv[]) {
  parallel::Init(argc, argv);

  int ranks_amount = parallel::RanksAmount();
  int curr_rank = parallel::CurrRank();

  const int N = 120;

  if (N % ranks_amount != 0) {
    throw std::runtime_error("main: N is not divisible by processes amount.");

    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  std::vector<int> curr_rank_vec(N / ranks_amount, curr_rank);

  std::vector<int> main_vec;
  if (curr_rank == 0) main_vec.resize(N);

  parallel::Gather(curr_rank_vec.data(), N / ranks_amount, MPI_INT,
                   main_vec.data(), N / ranks_amount, MPI_INT);

  if (curr_rank == 0) VectorToFile(main_vec, "result.txt");

  parallel::Finalize();
}
