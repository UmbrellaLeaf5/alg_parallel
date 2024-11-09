#include <cmath>
#include <cstdlib>
#include <iomanip>

#include "parallel.hpp"

int main(int argc, char *argv[]) {
  parallel::Init(argc, argv);

  int ranks_amount = parallel::RanksAmount();
  int curr_rank = parallel::CurrRank();

  if (argc != 2) parallel::Error("Usage: .exe file n points.");

  int N, count = 0;
  double epsilon = 1e-6, h, tau, delta_max_j, delta_max_all;

  try {
    N = std::atoi(argv[1]);
  } catch (...) {
    parallel::Error("Usage: .exe file n points.");
  }

  if (curr_rank == 0) {
    if (N <= 0)
      parallel::Error("N should be positive!");
    else
      std::cout << "Set N to " << N << "." << std::endl;
  }

  h = 1.0 / N;
  tau = 0.5 * (h * h);

  int beg_j = 1 + curr_rank * (N - 1) / ranks_amount;
  int end_j = 1 + (curr_rank + 1) * (N - 1) / ranks_amount;
  int N_curr_rank = end_j - beg_j + 1;

  std::vector<double> U(N_curr_rank + 1, 0.0);
  std::vector<double> U_new(N_curr_rank + 1, 0.0);
  std::vector<double> U_all;

  if (curr_rank == 0) {
    U_all.resize(N + 1, 0.0);

    U_all[0] = 1.0;
    U_all[N] = 0.0;

    U[0] = U_new[0] = 1.0;
  }

  for (;; count++) {
    delta_max_j = 0.0;

    for (int i = 1; i < N_curr_rank; i++) {
      U_new[i] = U[i] + (tau / (h * h)) * (U[i - 1] - 2 * U[i] + U[i + 1]);

      delta_max_j = std::fmax(delta_max_j, std::fabs(U_new[i] - U[i]));
    }

    parallel::Operation(delta_max_j, delta_max_all, MPI_DOUBLE, MPI_MAX);
    parallel::Broadcast(delta_max_all, MPI_DOUBLE);

    if (delta_max_all < epsilon) break;

    if (curr_rank > 0) {
      parallel::Send(U_new[1], MPI_DOUBLE, curr_rank - 1);

      parallel::ReceiveIgnoreStatus(U_new[0], MPI_DOUBLE, curr_rank - 1);
    }

    if (curr_rank < ranks_amount - 1) {
      parallel::Send(U_new[N_curr_rank - 1], MPI_DOUBLE, curr_rank + 1);

      parallel::ReceiveIgnoreStatus(U_new[N_curr_rank], MPI_DOUBLE,
                                    curr_rank + 1);
    }

    U = U_new;
  }

  int N_rank_number = N_curr_rank - 1;

  if (curr_rank == 0) {
    std::vector<int> counts(ranks_amount);
    parallel::Gather(&N_rank_number, 1, MPI_INT, counts.data(), 1, MPI_INT);

    std::vector<int> displacements(ranks_amount);

    for (int i = 0; i < ranks_amount - 1; i++) {
      if (i == 0) displacements[i] = 0;
      displacements[i + 1] = displacements[i] + counts[i];
    }

    parallel::GatherVarious(&U_new[1], MPI_DOUBLE, &U_all[1], MPI_DOUBLE,
                            counts.data(), displacements.data(), N_rank_number);
    // parallel::GatherVarious(U_new.data() + 1, MPI_DOUBLE, U_all.data() + 1,
    //                         MPI_DOUBLE, counts, displacements,
    //                         N_rank_number);

  } else {
    parallel::Gather(N_rank_number, MPI_INT, N_curr_rank, MPI_INT);

    parallel::GatherVarious(&U_new[1], MPI_DOUBLE, &U[1], MPI_DOUBLE,
                            &N_curr_rank, &N_curr_rank, N_rank_number);
    // parallel::GatherVarious(U_new.data() + 1, MPI_DOUBLE, U.data() + 1,
    //                         MPI_DOUBLE, &N_curr_rank, &N_curr_rank,
    //                         N_rank_number);
  }

  if (curr_rank == 0) VectorToFileWithPrecision(U_all);

  parallel::Finalize();

  return 0;
}
