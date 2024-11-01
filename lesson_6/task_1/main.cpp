#include <fstream>
#include <iostream>
#include <numeric>

#include "parallel.hpp"

int main(int argc, char* argv[]) {
  parallel::Init(argc, argv);

  int ranks_amount = parallel::RanksAmount();
  int curr_rank = parallel::CurrRank();

  std::vector<int> rank_vec(curr_rank + 1, curr_rank + 1);

  std::vector<int> amounts_vec(ranks_amount);
  // std::iota(amounts_vec.begin(), amounts_vec.end(), 1);
  for (std::size_t i = 0; i < static_cast<std::size_t>(ranks_amount); i++)
    amounts_vec[i] = i + 1;

  std::vector<int> res_vec(ranks_amount * ranks_amount);
  for (std::size_t i = 0; i < res_vec.size(); i++) res_vec[i] = 0;

  std::vector<int> displacements(ranks_amount);
  for (std::size_t i = 0; i < displacements.size(); i++)
    displacements[i] = i * (i + 1);

  parallel::GatherVarious(rank_vec, MPI_INT, res_vec, MPI_INT, amounts_vec,
                          displacements);

  if (curr_rank == 0) VectorToFile(res_vec);

  parallel::Finalize();
}
