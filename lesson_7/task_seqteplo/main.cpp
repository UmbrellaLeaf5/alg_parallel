#include <cmath>
#include <iomanip>
#include <iostream>

#include "parallel.hpp"

int main(int argc, char* argv[]) {
  parallel::Init(argc, argv);

  int ranks_amount = parallel::RanksAmount();
  int curr_rank = parallel::CurrRank();

  double delta_max_all = 0, h, tau, delta_max_j = 0.0, epsilon = 1.e-6;
  int count = 0;

  if (argc != 2) parallel::Error("Usage: .exe file n points.");

  int N;

  try {
    N = std::atoi(argv[1]);
  } catch (...) {
    parallel::Error("Usage: .exe file n points.");
  }

  if (curr_rank == 0)
    if (N <= 0)
      parallel::Error("N should be positive!");
    else
      std::cout << "Set N to " << N << "." << std::endl;

  h = 1.0 / N;
  tau = 0.5 * (h * h);

  int N_curr_rank = (N - 1) / ranks_amount;
  int beg_j = N_curr_rank * curr_rank + 1;
  int end_j = N_curr_rank * (curr_rank + 1) + 1;

  if (curr_rank == ranks_amount - 1) {
    end_j = N;
    N_curr_rank = end_j - beg_j;
  }

  std::vector<double> U(N_curr_rank + 2, 0.0), U_new(N_curr_rank + 2, 0.0);

  if (curr_rank == 0) U_new[0] = U[0] = 1.0;

  if (curr_rank == ranks_amount - 1) U_new[N_curr_rank] = U[N_curr_rank] = 0.0;

  std::cout << "CURR: " << curr_rank << "; " << beg_j << "; " << end_j
            << std::endl;

  for (;;) {
    // std::cout << "COUNT: " << curr_rank << " " << count << std::endl;

    delta_max_j = 0.0;

    for (int i = 1; i < N_curr_rank; i++) {
      U_new[i] = U[i] + (tau / (h * h)) * (U[i - 1] - 2 * U[i] + U[i + 1]);

      delta_max_j = std::fmax(delta_max_j, std::fabs(U_new[i] - U[i]));
    }

    // std::cout << "Delta MAX on " << curr_rank << " " << delta_max_j
    //           << std::endl;

    parallel::Operation(delta_max_j, delta_max_all, MPI_DOUBLE, MPI_MAX);
    // std::cout << "REDUCE: SUCCESS: " << delta_max_all << std::endl;
    parallel::Broadcast(delta_max_all, MPI_DOUBLE);
    // std::cout << "BCAST: SUCCESS" << std::endl;

    if (delta_max_all < epsilon) break;
    count++;

    // std::cout << "U_new CYCLE: " << curr_rank << "; " << U_new << std::endl;

    if (curr_rank < ranks_amount - 1) {
      parallel::Send(U_new[N_curr_rank - 1], MPI_DOUBLE, curr_rank + 1);

      parallel::ReceiveIgnoreStatus(U_new[N_curr_rank], MPI_DOUBLE,
                                    curr_rank + 1);
    }
    if (curr_rank > 0) {
      parallel::Send(U_new[1], MPI_DOUBLE, curr_rank - 1);

      parallel::ReceiveIgnoreStatus(U_new[0], MPI_DOUBLE, curr_rank - 1);
    }

    U = U_new;
  }

  std::cout << std::fixed << std::setprecision(8)
            << "U_new AFTER: " << curr_rank << "; " << U_new << std::endl;
  std::cout << std::fixed << std::setprecision(8) << "Delta MAX: " << curr_rank
            << "; " << delta_max_all << std::endl;

  parallel::Finalize();
}
