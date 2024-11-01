#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "parallel.hpp"

int main(int argc, char* argv[]) {
  parallel::Init(argc, argv);

  int ranks_amount = parallel::RanksAmount();
  int curr_rank = parallel::CurrRank();

  const int N = 120;

  if (N % ranks_amount != 0)
    parallel::Error("N is not divisible by processes amount");

  std::vector<int> curr_rank_vec(N / ranks_amount, curr_rank);

  std::vector<int> main_vec;
  if (curr_rank == 0) main_vec.resize(N);

  parallel::Gather(curr_rank_vec.data(), N / ranks_amount, MPI_INT,
                   main_vec.data(), N / ranks_amount, MPI_INT);

  if (curr_rank == 0) VectorToFile(main_vec);

  parallel::Finalize();
}
