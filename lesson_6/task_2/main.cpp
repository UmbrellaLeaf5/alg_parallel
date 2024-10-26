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

  int dis = 0;
  for (int i = 0; i < ranks_amount; ++i) {
    displacements[i] = dis;
    dis += amounts_vec[i];
  }

  for (std::size_t i = 0; i < size; i++) rank_vec[i] = sqrt(curr_rank);

  std::vector<double> res_vec(dis);

  std::cout << rank_vec << std::endl
            << size << std::endl
            << res_vec << std::endl
            << amounts_vec << std::endl
            << displacements << std::endl;

  parallel::CheckSuccess(MPI_Gatherv(
      rank_vec.data(), size, MPI_DOUBLE, res_vec.data(), amounts_vec.data(),
      displacements.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD));

  if (curr_rank == 0) {
    std::ofstream outfile("results.txt");
    for (int i = 0; i < ranks_amount; i++) {
      for (int j = 0; j < amounts_vec[i]; j++)
        outfile << res_vec[displacements[i] + j] << " ";

      outfile << std::endl;
    }

    outfile.close();
  }

  parallel::Finalize();
}
