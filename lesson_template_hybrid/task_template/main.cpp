#include <mpi.h>
#include <omp.h>
#include <stdio.h>

#include <iostream>
#include <string>

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

#pragma omp parallel
  {
  }

  MPI_Finalize();
}
