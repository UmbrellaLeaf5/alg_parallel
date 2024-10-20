#pragma once

#include <mpi.h>

#include <climits>
#include <stdexcept>
#include <vector>

namespace parallel {

/// @brief тег для сообщений MPI.
#define STANDARD_PARALLEL_TAG 35817

/**
 * @brief Завершает выполнение процесса MPI, если состояние не равно
 * MPI_SUCCESS.
 * @param state: состояние выполнения MPI.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 */
inline void CheckSuccess(int state, MPI_Comm comm = MPI_COMM_WORLD) {
  if (state != MPI_SUCCESS) MPI_Abort(comm, state);
}

/**
 * @brief Инициализирует среду MPI.
 * @param argc: количество аргументов командной строки.
 * @param argv: массив аргументов командной строки.
 */
inline void Init(int argc, char *argv[]) {
  parallel::CheckSuccess(MPI_Init(&argc, &argv));
}

/// @brief Завершает работу со средой MPI.
inline void Finalize() { parallel::CheckSuccess(MPI_Finalize()); }

/**
 * @brief Возвращает количество процессов в коммуникаторе.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 * @return количество процессов.
 */
inline int RanksAmount(MPI_Comm comm = MPI_COMM_WORLD) {
  int ranks_amount = 0;
  parallel::CheckSuccess(MPI_Comm_size(comm, &ranks_amount));

  return ranks_amount;
}

/**
 * @brief Возвращает ранг (индекс) текущего процесса.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 * @return ранг (индекс) текущего процесса.
 */
inline int CurrRank(MPI_Comm comm = MPI_COMM_WORLD) {
  int curr_rank = 0;
  parallel::CheckSuccess(MPI_Comm_rank(comm, &curr_rank));

  return curr_rank;
}

/**
 * @brief Отправляет значение по сети MPI.
 * @tparam T: тип отправляемого значения.
 * @param value: отправляемое значение.
 * @param datatype: тип данных значения.
 * @param to_rank: ранг получателя.
 * @param tag: тег сообщения. По умолчанию STANDARD_PARALLEL_TAG.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 */
template <typename T>
inline void Send(T &value, MPI_Datatype datatype, int to_rank,
                 int tag = STANDARD_PARALLEL_TAG,
                 MPI_Comm comm = MPI_COMM_WORLD) {
  parallel::CheckSuccess(MPI_Send(&value, 1, datatype, to_rank, tag, comm));
}

/**
 * @brief Отправляет массив по сети MPI.
 * @tparam T: тип элементов массива.
 * @param arr: массив, который нужно отправить.
 * @param arr_len: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param to_rank: ранг получателя.
 * @param tag: тег сообщения. По умолчанию STANDARD_PARALLEL_TAG.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 * @throw std::invalid_argument("parallel::Send: arr_len should be
 * non-negative.");
 */
template <typename T>
inline void Send(T *arr, int arr_len, MPI_Datatype datatype, int to_rank,
                 int tag = STANDARD_PARALLEL_TAG,
                 MPI_Comm comm = MPI_COMM_WORLD) {
  if (arr_len <= 0)
    throw std::invalid_argument(
        "parallel::Send: arr_len should be non-negative.");

  parallel::CheckSuccess(MPI_Send(arr, arr_len, datatype, to_rank, tag, comm));
}

/**
 * @brief Отправляет вектор по сети MPI.
 * @tparam T: тип элементов вектора.
 * @param arr: вектор, который нужно отправить.
 * @param datatype: тип данных элементов вектора.
 * @param to_rank: ранг получателя.
 * @param tag: тег сообщения. По умолчанию STANDARD_PARALLEL_TAG.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 * @throw std::invalid_argument("parallel::Send: vector is too big (size >
 * INT_MAX).");
 */
template <typename T>
inline void Send(const std::vector<T> &vec, MPI_Datatype datatype, int to_rank,
                 int tag = STANDARD_PARALLEL_TAG,
                 MPI_Comm comm = MPI_COMM_WORLD) {
  if (vec.size() > INT_MAX)
    throw std::invalid_argument(
        "parallel::Send: vector is too big (size > INT_MAX).");

  parallel::Send(vec.data(), static_cast<int>(vec.size()), datatype, to_rank,
                 tag, comm);
}

/**
 * @brief Получает значение по сети MPI.
 * @tparam T: тип получаемого значения.
 * @param value: получаемое значение.
 * @param datatype: тип данных значения.
 * @param status: статус сообщения.
 * @param from_rank: ранг отправителя. По умолчанию MPI_ANY_SOURCE.
 * @param tag: тег сообщения. По умолчанию STANDARD_PARALLEL_TAG.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 */
template <typename T>
inline void Receive(T &value, MPI_Datatype datatype, MPI_Status &status,
                    int from_rank = MPI_ANY_SOURCE,
                    int tag = STANDARD_PARALLEL_TAG,
                    MPI_Comm comm = MPI_COMM_WORLD) {
  parallel::CheckSuccess(
      MPI_Recv(&value, 1, datatype, from_rank, tag, comm, &status));
}

/**
 * @brief Получает массив по сети MPI.
 * @tparam T: тип элементов массива.
 * @param arr: массив, в который нужно получить данные.
 * @param arr_len: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param status: статус сообщения.
 * @param from_rank: ранг отправителя. По умолчанию MPI_ANY_SOURCE.
 * @param tag: тег сообщения. По умолчанию STANDARD_PARALLEL_TAG.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 * @throw std::invalid_argument("parallel::Receive: arr_len should be
 * non-negative.");
 */
template <typename T>
inline void Receive(T *arr, int arr_len, MPI_Datatype datatype,
                    MPI_Status &status, int from_rank = MPI_ANY_SOURCE,
                    int tag = STANDARD_PARALLEL_TAG,
                    MPI_Comm comm = MPI_COMM_WORLD) {
  if (arr_len <= 0)
    throw std::invalid_argument(
        "parallel::Receive: arr_len should be non-negative.");

  parallel::CheckSuccess(
      MPI_Recv(arr, arr_len, datatype, from_rank, tag, comm, &status));
}

/**
 * @brief Получает вектор по сети MPI.
 * @tparam T: тип элементов вектора.
 * @param arr: вектор, в который нужно получить данные.
 * @param datatype: тип данных элементов вектора.
 * @param status: статус сообщения.
 * @param from_rank: ранг отправителя. По умолчанию MPI_ANY_SOURCE.
 * @param tag: тег сообщения. По умолчанию STANDARD_PARALLEL_TAG.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 * @throw std::invalid_argument("parallel::Receive: vector is too big (size >
 * INT_MAX).");
 */
template <typename T>
inline void Receive(std::vector<T> &vec, MPI_Datatype datatype,
                    MPI_Status &status, int from_rank = MPI_ANY_SOURCE,
                    int tag = STANDARD_PARALLEL_TAG,
                    MPI_Comm comm = MPI_COMM_WORLD) {
  if (vec.size() > INT_MAX)
    throw std::invalid_argument(
        "parallel::Receive: vector is too big (size > INT_MAX).");

  parallel::Receive(vec.data(), static_cast<int>(vec.size()), datatype,
                    from_rank, tag, comm, &status);
}

/**
 * @brief Рассылает значение от процесса с указанным рангом по сети MPI.
 * @tparam T: тип рассылаемого значения.
 * @param value: рассылаемое значение.
 * @param datatype: тип данных значения.
 * @param from_rank: ранг процесса рассылки. По умолчанию 0.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 */
template <typename T>
inline void Broadcast(T &value, MPI_Datatype datatype, int from_rank = 0,
                      MPI_Comm comm = MPI_COMM_WORLD) {
  parallel::CheckSuccess(MPI_Bcast(&value, 1, datatype, from_rank, comm));
}

/**
 * @brief Рассылает массив от процесса с указанным рангом по сети MPI.
 * @tparam T: тип элементов рассылаемого массива.
 * @param arr: рассылаемый массив.
 * @param arr_len: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param from_rank: ранг процесса рассылки. По умолчанию 0.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 * @throw std::invalid_argument("parallel::Broadcast: arr_len should be
 * non-negative.");
 */
template <typename T>
inline void Broadcast(T *arr, int arr_len, MPI_Datatype datatype,
                      int from_rank = 0, MPI_Comm comm = MPI_COMM_WORLD) {
  if (arr_len <= 0)
    throw std::invalid_argument(
        "parallel::Broadcast: arr_len should be non-negative.");

  parallel::CheckSuccess(MPI_Bcast(arr, arr_len, datatype, from_rank, comm));
}

/**
 * @brief Рассылает вектор от процесса с указанным рангом по сети MPI.
 * @tparam T: тип элементов рассылаемого вектора.
 * @param vec: рассылаемый вектор.
 * @param datatype: тип данных элементов вектора.
 * @param from_rank: ранг процесса рассылки. По умолчанию 0.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 * @throw std::invalid_argument("parallel::Broadcast: vector is too big (size >
 * INT_MAX).");
 */
template <typename T>
inline void Broadcast(std::vector<T> vec, MPI_Datatype datatype,
                      int from_rank = 0, MPI_Comm comm = MPI_COMM_WORLD) {
  if (vec.size() > INT_MAX)
    throw std::invalid_argument(
        "parallel::Broadcast: vector is too big (size > INT_MAX).");

  parallel::Broadcast(vec.data(), static_cast<int>(vec.size()), datatype,
                      from_rank, comm);
}

/**
 * @brief Выполняет операцию над значением и отправляет результат на
 * указанный процесс в сети MPI.
 * @tparam T: тип значения.
 * @param from_value: исходное значение, над которым выполняется операция.
 * @param to_value: значение, куда будет записан результат операции.
 * @param datatype: тип данных значения.
 * @param op: операция MPI, которая будет выполнена.
 * @param to_rank: ранг процесса результата. По умолчанию 0.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 */
template <typename T>
inline void Operation(const T &from_value, T &to_value, MPI_Datatype datatype,
                      MPI_Op op, int to_rank = 0,
                      MPI_Comm comm = MPI_COMM_WORLD) {
  parallel::CheckSuccess(
      MPI_Reduce(&from_value, &to_value, 1, datatype, op, to_rank, comm));
}

/**
 * @brief Выполняет операцию над массивом значений и отправляет результат на
 * указанный процесс в сети MPI.
 * @tparam T: тип значения в массиве.
 * @param from_arr: исходный массив значений, над которым выполняется операция.
 * @param to_arr: массив, куда будет записан результат операции.
 * @param arr_len: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param op: операция MPI, которая будет выполнена.
 * @param to_rank: ранг процесса результата. По умолчанию 0.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 * @throw std::invalid_argument("parallel::Operation: arr_len should be
 * non-negative.");
 */
template <typename T>
inline void Operation(const T *from_arr, T *to_arr, int arr_len,
                      MPI_Datatype datatype, MPI_Op op, int to_rank = 0,
                      MPI_Comm comm = MPI_COMM_WORLD) {
  if (arr_len <= 0)
    throw std::invalid_argument(
        "parallel::Operation: arr_len should be non-negative.");

  parallel::CheckSuccess(
      MPI_Reduce(from_arr, to_arr, arr_len, datatype, op, to_rank, comm));
}

/**
 * @brief Выполняет операцию над вектором значений и отправляет результат на
 * указанный процесс в сети MPI.
 * @tparam T: тип значения в векторе.
 * @param from_vec: исходный вектор значений, над которым выполняется операция.
 * @param to_vec: вектор, куда будет записан результат операции.
 * @param datatype: тип данных в векторе.
 * @param op: операция MPI, которая будет выполнена.
 * @param to_rank: ранг процесса результата. По умолчанию 0.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 * @throw std::invalid_argument("parallel::Operation: vectors of different
 * sizes.");
 * @throw std::invalid_argument("parallel::Operation: vector is too big (size >
 * INT_MAX).");
 */
template <typename T>
inline void Operation(const std::vector<T> &from_vec, std::vector<T> &to_vec,
                      MPI_Datatype datatype, MPI_Op op, int to_rank = 0,
                      MPI_Comm comm = MPI_COMM_WORLD) {
  if (from_vec.size() != to_vec.size())
    throw std::invalid_argument(
        "parallel::Operation: vectors of different sizes.");

  if (from_vec.size() > INT_MAX)
    throw std::invalid_argument(
        "parallel::Operation: vector is too big (size > INT_MAX).");

  parallel::Operation(from_vec.data(), to_vec.data(),
                      static_cast<int>(from_vec.size();), datatype, op, to_rank,
                      comm);
}

/**
 * @brief Собирать значения от всех процессов в сети MPI в одном процессе.
 * @tparam T: тип значения.
 * @param from_value: значение, которое будет отправлено от текущего процесса.
 * @param from_value_datatype: тип данных `from_value`.
 * @param to_value: ссылка на значение, куда будет записан результат сбора на
 * процессе `to_rank`.
 * @param to_value_datatype: тип данных `to_value`.
 * @param to_rank: ранг процесса результата. По умолчанию 0.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 */
template <typename T>
inline void Gather(const T &from_value, MPI_Datatype from_value_datatype,
                   T &to_value, MPI_Datatype to_value_datatype, int to_rank = 0,
                   MPI_Comm comm = MPI_COMM_WORLD) {
  parallel::CheckSuccess(MPI_Gather(&from_value, 1, from_value_datatype,
                                    &to_value, 1, to_value_datatype, to_rank,
                                    comm));
}

/**
 * @brief Собирает массивы от всех процессов в сети MPI в одном процессе.
 * @tparam T: тип значения в массиве.
 * @param from_arr: массив, который будет отправлен от текущего процесса.
 * @param from_arr_len: количество элементов в массиве `from_arr`.
 * @param from_arr_datatype: тип данных элементов массива `from_arr`.
 * @param to_arr: массив, куда будет записан результат сбора на процессе
 * `to_rank`.
 * @param to_arr_len: количество элементов в массиве `to_arr`.
 * @param to_arr_datatype: тип данных элементов массива `to_arr`.
 * @param to_rank: ранг процесса результата. По умолчанию 0.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 * @throw std::invalid_argument("parallel::Gather: arr_len should be
 * non-negative.");
 */
template <typename T>
inline void Gather(const T *from_arr, int from_arr_len,
                   MPI_Datatype from_arr_datatype, T *to_arr, int to_arr_len,
                   MPI_Datatype to_arr_datatype, int to_rank = 0,
                   MPI_Comm comm = MPI_COMM_WORLD) {
  if (from_arr_len < 0 || to_arr_len < 0)
    throw std::invalid_argument(
        "parallel::Gather: arr_len should be non-negative.");

  parallel::CheckSuccess(MPI_Gather(from_arr, from_arr_len, from_arr_datatype,
                                    to_arr, to_arr_len, to_arr_datatype,
                                    to_rank, comm));
}

/**
 * @brief Собирает векторы от всех процессов в сети MPI в одном процессе.
 * @tparam T: тип значения в массиве.
 * @param from_vec: массив, который будет отправлен от текущего процесса.
 * @param from_vec_datatype: тип данных элементов массива `from_arr`.
 * @param to_vec: массив, куда будет записан результат сбора на процессе
 * `to_rank`.
 * @param to_vec_datatype: тип данных элементов массива `to_arr`.
 * @param to_rank: ранг процесса результата. По умолчанию 0.
 * @param comm: коммуникатор MPI. По умолчанию MPI_COMM_WORLD.
 * @throw std::invalid_argument("parallel::Gather: vector is too big (size >
 * INT_MAX).");
 */
template <typename T>
inline void Gather(const std::vector<T> &from_vec,
                   MPI_Datatype from_vec_datatype, std::vector<T> &to_vec,
                   MPI_Datatype to_vec_datatype, int to_rank = 0,
                   MPI_Comm comm = MPI_COMM_WORLD) {
  if (from_vec.size() > INT_MAX || to_vec.size() > INT_MAX)
    throw std::invalid_argument(
        "parallel::Gather: vector is too big (size > INT_MAX).");

  parallel::Gather(from_vec.data(), static_cast<int>(from_vec.size()),
                   from_vec_datatype, to_vec.data(),
                   static_cast<int>(to_vec.size()), to_vec_datatype, to_rank,
                   comm);
}

}  // namespace parallel