#include <iostream>

#include "parallel.hpp"

int main(int argc, char* argv[]) {
  parallel::Init(argc, argv);

  [[maybe_unused]] int ranks_amount = parallel::RanksAmount();
  [[maybe_unused]] int curr_rank = parallel::CurrRank();

  parallel::Finalize();
}
