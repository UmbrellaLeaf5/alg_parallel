#include <mpi.h>
#include <omp.h>
#include <stdio.h>

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Записывает вектор в файл
 * @tparam Type: тип данных в векторе
 * @param vec: вектор
 * @param precision: точность
 * @param file_name: имя файла
 */
void VectorToFileWithPrecision(const std::vector<double>& vec,
                               int precision = 6,
                               const std::string& file_name = "results.txt") {
  std::ofstream out(file_name.c_str());

  if (!out.is_open()) {
    std::cerr << "VectorToFileWithPrecision: file opening error." << std::endl;
    return;
  }

  for (size_t i = 0; i < vec.size(); i++)
    out << std::fixed << std::setprecision(precision) << vec[i] << std::endl;

  out.close();
}

/**
 * @brief Завершает выполнение процесса MPI, если состояние не равно
 * MPI_SUCCESS.
 * @param state: состояние выполнения MPI.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 */
void CheckSuccess(int state, MPI_Comm comm = MPI_COMM_WORLD) {
  if (state != MPI_SUCCESS) MPI_Abort(comm, state);
}

/**
 * @brief Аварийно завершает работу со средой MPI, выводя ошибку в поток
 * std::cerr.
 * @param error_text: текст ошибки.
 * @param state: состояние выполнения MPI. По умолчанию 1.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 */
void Error(const std::string& error_text, int state = 1,
           MPI_Comm comm = MPI_COMM_WORLD) {
  std::cerr << "Error! Aborting because of: " << error_text << std::endl;
  MPI_Abort(comm, state);
}

int main(int argc, char* argv[]) {
  int provided, required = MPI_THREAD_SERIALIZED;

  CheckSuccess(MPI_Init_thread(&argc, &argv, required, &provided));

  if (provided != required)
    Error("The required threading support level is not equal demanded.");

  int ranks_amount, curr_rank;
  CheckSuccess(MPI_Comm_size(MPI_COMM_WORLD, &ranks_amount));
  CheckSuccess(MPI_Comm_rank(MPI_COMM_WORLD, &curr_rank));

  if (argc != 2) Error("Usage: .exe file n points.");

  int N, count = 0;
  double epsilon = 1e-6, h, tau, delta_max;

  try {
    N = std::atoi(argv[1]);
  } catch (...) {
    Error("Usage: .exe file n points.");
  }

  if (curr_rank == 0) {
    if (N <= 0)
      Error("N should be positive!");
    else
      std::cout << "Set N to " << N << "." << std::endl;
  }

  h = 1.0 / N;
  tau = 0.5 * (h * h);

  int beg_j = 1 + curr_rank * (N - 1) / ranks_amount;
  int end_j = 1 + (curr_rank + 1) * (N - 1) / ranks_amount;
  int N_curr_rank = end_j - beg_j + 1;

  int num_procs = omp_get_num_procs();

  std::vector<double> U(N_curr_rank + 1, 0.0);
  std::vector<double> U_new(N_curr_rank + 1, 0.0);
  std::vector<double> U_all;

  std::vector<double> proc_max_array(num_procs, 0.0);

  if (curr_rank == 0) {
    U_all.resize(N + 1, 0.0);

    U_all[0] = 1.0;
    U_all[N] = 0.0;

    U[0] = U_new[0] = 1.0;
  }

  for (double delta_max_all;; count++) {
    delta_max = 0.0;

#pragma omp parallel for shared(U_new, U)
    for (int i = 1; i < N_curr_rank; i++) {
      U_new[i] = U[i] + (tau / (h * h)) * (U[i - 1] - 2 * U[i] + U[i + 1]);

      int thread_num = omp_get_thread_num();
      double delta = std::abs(U_new[i] - U[i]);

      if (delta > proc_max_array[thread_num])
        proc_max_array[thread_num] = delta;
    }

    delta_max = proc_max_array[0];
    for (int i = 0; i < num_procs; i++) {
      if (delta_max < proc_max_array[i]) delta_max = proc_max_array[i];
      proc_max_array[i] = 0;
    }

    CheckSuccess(MPI_Reduce(&delta_max, &delta_max_all, 1, MPI_DOUBLE, MPI_MAX,
                            0, MPI_COMM_WORLD));
    CheckSuccess(MPI_Bcast(&delta_max_all, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD));

    if (delta_max_all < epsilon) break;

    if (curr_rank > 0) {
      CheckSuccess(MPI_Send(&U_new[1], 1, MPI_DOUBLE, curr_rank - 1, 35817,
                            MPI_COMM_WORLD));

      CheckSuccess(MPI_Recv(&U_new[0], 1, MPI_DOUBLE, curr_rank - 1, 35817,
                            MPI_COMM_WORLD, MPI_STATUS_IGNORE));
    }

    if (curr_rank < ranks_amount - 1) {
      CheckSuccess(MPI_Send(&U_new[N_curr_rank - 1], 1, MPI_DOUBLE,
                            curr_rank + 1, 35817, MPI_COMM_WORLD));

      CheckSuccess(MPI_Recv(&U_new[N_curr_rank], 1, MPI_DOUBLE, curr_rank + 1,
                            35817, MPI_COMM_WORLD, MPI_STATUS_IGNORE));
    }

    std::swap(U, U_new);
  }

  int N_rank_number = N_curr_rank - 1;

  if (curr_rank == 0) {
    std::vector<int> counts(ranks_amount);
    CheckSuccess(MPI_Gather(&N_rank_number, 1, MPI_INT, counts.data(), 1,
                            MPI_INT, 0, MPI_COMM_WORLD));

    std::vector<int> displacements(ranks_amount, 0);

    for (int i = 1; i < ranks_amount - 1; i++)
      displacements[i + 1] = displacements[i] + counts[i];

    CheckSuccess(MPI_Gatherv(&U_new[1], N_rank_number, MPI_DOUBLE, &U_all[1],
                             counts.data(), displacements.data(), MPI_DOUBLE, 0,
                             MPI_COMM_WORLD));

    VectorToFileWithPrecision(U_all);

  } else {
    CheckSuccess(MPI_Gather(&N_rank_number, 1, MPI_INT, &N_curr_rank, 1,
                            MPI_INT, 0, MPI_COMM_WORLD));

    CheckSuccess(MPI_Gatherv(&U_new[1], N_rank_number, MPI_DOUBLE, &U[1],
                             &N_curr_rank, &N_curr_rank, MPI_DOUBLE, 0,
                             MPI_COMM_WORLD));
  }

  std::cout << "Steps: " << count << std::endl;

  MPI_Finalize();
}
