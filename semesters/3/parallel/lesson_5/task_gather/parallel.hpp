#pragma once

#include <mpi.h>

namespace parallel {

/// @brief тег для сообщений MPI.
#define STANDARD_PARALLEL_TAG 35817

/**
 * @brief Завершает выполнение процесса MPI, если состояние не равно
 * MPI_SUCCESS.
 * @param state: состояние выполнения MPI.
 * @param comm: коммуникатор MPI. По умолчанию используется MPI_COMM_WORLD.
 */
void AbortTest(int state, MPI_Comm comm = MPI_COMM_WORLD) {
  if (state != MPI_SUCCESS) MPI_Abort(comm, state);
}

/**
 * @brief Инициализирует среду MPI.
 * @param argc: количество аргументов командной строки.
 * @param argv: массив аргументов командной строки.
 */
void Init(int argc, char *argv[]) { AbortTest(MPI_Init(&argc, &argv)); }

/// @brief Завершает работу со средой MPI.
void Finalize() { AbortTest(MPI_Finalize()); }

/**
 * @brief Возвращает количество процессов в коммуникаторе.
 * @param comm: коммуникатор MPI. По умолчанию используется MPI_COMM_WORLD.
 * @return количество процессов.
 */
int RanksAmount(MPI_Comm comm = MPI_COMM_WORLD) {
  int ranks_amount = 0;
  AbortTest(MPI_Comm_size(comm, &ranks_amount));

  return ranks_amount;
}

/**
 * @brief Возвращает ранг (индекс) текущего процесса.
 * @param comm: коммуникатор MPI. По умолчанию используется MPI_COMM_WORLD.
 * @return ранг (индекс) текущего процесса.
 */
int CurrRank(MPI_Comm comm = MPI_COMM_WORLD) {
  int curr_rank = 0;
  AbortTest(MPI_Comm_rank(comm, &curr_rank));

  return curr_rank;
}

/**
 * @brief Отправляет значение по сети MPI.
 * @tparam T: тип отправляемого значения.
 * @param value: отправляемое значение.
 * @param datatype: тип данных значения.
 * @param to_rank: ранг получателя.
 * @param tag: тег сообщения. По умолчанию используется STANDARD_PARALLEL_TAG.
 * @param comm: коммуникатор MPI. По умолчанию используется MPI_COMM_WORLD.
 */
template <typename T>
void Send(T &value, MPI_Datatype datatype, int to_rank,
          int tag = STANDARD_PARALLEL_TAG, MPI_Comm comm = MPI_COMM_WORLD) {
  AbortTest(MPI_Send(&value, 1, datatype, to_rank, tag, comm));
}

/**
 * @brief Отправляет массив по сети MPI.
 * @tparam T: тип элементов массива.
 * @param array: массив, который нужно отправить.
 * @param count: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param to_rank: ранг получателя.
 * @param tag: тег сообщения. По умолчанию используется STANDARD_PARALLEL_TAG.
 * @param comm: коммуникатор MPI. По умолчанию используется MPI_COMM_WORLD.
 */
template <typename T>
void Send(T *array, int count, MPI_Datatype datatype, int to_rank,
          int tag = STANDARD_PARALLEL_TAG, MPI_Comm comm = MPI_COMM_WORLD) {
  AbortTest(MPI_Send(array, count, datatype, to_rank, tag, comm));
}

/**
 * @brief Получает значение по сети MPI.
 * @tparam T: тип получаемого значения.
 * @param value: получаемое значение.
 * @param datatype: тип данных значения.
 * @param status: статус сообщения.
 * @param from_rank: ранг отправителя. По умолчанию используется MPI_ANY_SOURCE.
 * @param tag: тег сообщения. По умолчанию используется STANDARD_PARALLEL_TAG.
 * @param comm: коммуникатор MPI. По умолчанию используется MPI_COMM_WORLD.
 */
template <typename T>
void Receive(T &value, MPI_Datatype datatype, MPI_Status &status,
             int from_rank = MPI_ANY_SOURCE, int tag = STANDARD_PARALLEL_TAG,
             MPI_Comm comm = MPI_COMM_WORLD) {
  AbortTest(MPI_Recv(&value, 1, datatype, from_rank, tag, comm, &status));
}

/**
 * @brief Получает массив по сети MPI.
 * @tparam T: тип элементов массива.
 * @param array: массив, в который нужно получить данные.
 * @param count: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param status: статус сообщения.
 * @param from_rank: ранг отправителя. По умолчанию используется MPI_ANY_SOURCE.
 * @param tag: тег сообщения. По умолчанию используется STANDARD_PARALLEL_TAG.
 * @param comm: коммуникатор MPI. По умолчанию используется MPI_COMM_WORLD.
 */
template <typename T>
void Receive(T *array, int count, MPI_Datatype datatype, MPI_Status &status,
             int from_rank = MPI_ANY_SOURCE, int tag = STANDARD_PARALLEL_TAG,
             MPI_Comm comm = MPI_COMM_WORLD) {
  AbortTest(MPI_Recv(array, count, datatype, from_rank, tag, comm, &status));
}

}  // namespace parallel