#include <mpi.h>
#include <stdlib.h>

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "parallel.hpp"
#include "utils.hpp"

int main(int argc, char* argv[]) {
  parallel::Init(argc, argv);

  int ranks_amount = parallel::RanksAmount();
  int curr_rank = parallel::CurrRank();

  srand(curr_rank + 1);

  int size = rand() % 7 + 1;

  std::vector<double> rank_vec(size);
  std::vector<int> displacements(ranks_amount);
  std::vector<int> amounts_vec(ranks_amount);

  parallel::Gather(&size, 1, MPI_INT, amounts_vec.data(), 1, MPI_INT);

  int displacement = 0;
  for (int i = 0; i < ranks_amount; ++i) {
    displacements[i] = displacement;
    displacement += amounts_vec[i];
  }

  for (std::size_t i = 0; i < static_cast<std::size_t>(size); i++)
    rank_vec[i] = sqrt(curr_rank);

  std::vector<double> res_vec(displacement);

  // std::cout << "rank_vec: " << rank_vec << std::endl
  //           << "size: " << size << std::endl
  //           << "res_vec: " << res_vec << std::endl
  //           << "amounts_vec: " << amounts_vec << std::endl
  //           << "displacements: " << displacements << std::endl
  //           << std::endl;

  parallel::CheckSuccess(MPI_Gatherv(
      rank_vec.data(), size, MPI_DOUBLE, res_vec.data(), amounts_vec.data(),
      displacements.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD));

  if (curr_rank == 0) {
    std::ofstream outfile(
        std::string(ToString(ranks_amount) + "_results.txt").c_str());

    for (int i = 0; i < ranks_amount; i++) {
      for (int j = 0; j < amounts_vec[i]; j++)
        outfile << res_vec[displacements[i] + j] << " ";

      outfile << std::endl;
    }

    outfile.close();
  }

  parallel::Finalize();
}
