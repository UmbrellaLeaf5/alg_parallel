#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "parallel.hpp"
#include "pi.hpp"

int main(int argc, char* argv[]) {
  parallel::Init(argc, argv);

  int ranks_amount = parallel::RanksAmount();
  int curr_rank = parallel::CurrRank();

  int N;

  if (curr_rank == 0) N = NumberFromFile("N.dat");

  parallel::Broadcast(N, MPI_INT);

  double part_of_pi;

  part_of_pi = PartOfPi(N, curr_rank * (N / ranks_amount),
                        (curr_rank + 1) * (N / ranks_amount));

  double pi = 0;

  parallel::Operation(&part_of_pi, &pi, 1, MPI_DOUBLE, MPI_SUM);

  if (curr_rank == 0) std::cout << "Pi: " << pi << std::endl;

  parallel::Finalize();
}
