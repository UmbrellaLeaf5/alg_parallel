#pragma once

#include <mpi.h>

#include "utils.hpp"

/// @brief тег для сообщений MPI.
#define PARALLEL_STANDARD_TAG 35817

#ifndef PARALLEL_MACROS_NEED_PRINT
#define PARALLEL_MACROS_NEED_PRINT false
#endif

#ifndef PARALLEL_MACROS_DEFAULT_VALUES

/**
 * @brief Аварийно завершает работу со средой MPI.
 * @param state: состояние выполнения MPI.
 * @param comm: коммуникатор MPI.
 */
#define parallel_Abort(state, comm) MPI_Abort(comm, state)

/**
 * @brief Завершает выполнение процесса MPI, если состояние не равно
 * MPI_SUCCESS.
 * @param state: состояние выполнения MPI.
 * @param comm: коммуникатор MPI.
 */
#define parallel_CheckSuccess(state, comm)           \
  {                                                  \
    int st = state;                                  \
    if (st != MPI_SUCCESS) parallel_Abort(st, comm); \
  }

/**
 * @brief Аварийно завершает работу со средой MPI, выводя ошибку в поток
 * std::cerr.
 * @param error_text: текст ошибки.
 * @param state: состояние выполнения MPI.
 * @param comm: коммуникатор MPI.
 */
#define parallel_Error(error_text, state, comm)                             \
  {                                                                         \
    std::cerr << "Error! Aborting because of: " << error_text << std::endl; \
    parallel_Abort(state, comm);                                            \
  }

/**
 * @brief Инициализирует среду MPI.
 * @param argc: количество аргументов командной строки.
 * @param argv: массив аргументов командной строки.
 * @param comm: коммуникатор MPI.
 */
#define parallel_Init(argc, argv, comm)                                  \
  {                                                                      \
    if (PARALLEL_MACROS_NEED_PRINT)                                      \
      std::cout << "parallel_Init with args: argc: " << argc             \
                << "; argv: " << argv << std::endl;                      \
    parallel_CheckSuccess(MPI_Init(&argc, &argv), comm);                 \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl; \
  }

/**
 * @brief Инициализирует среду MPI.
 * @param comm: коммуникатор MPI.
 */
#define parallel_Finalize(comm)                                          \
  {                                                                      \
    parallel_CheckSuccess(MPI_Finalize(), comm);                         \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl; \
  }

/**
 * @brief Считает количество процессов в коммуникаторе.
 * @param ranks_amount: количество процессов в коммуникаторе.
 * @param comm: коммуникатор MPI.
 */
#define parallel_RanksAmount(ranks_amount, comm)                         \
  {                                                                      \
    if (PARALLEL_MACROS_NEED_PRINT)                                      \
      std::cout << "parallel_RanksAmount with args: comm:b" << comm      \
                << std::endl;                                            \
    parallel_CheckSuccess(MPI_Comm_size(comm, &ranks_amount), comm);     \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl; \
  }

/**
 * @brief Считает ранг (индекс) текущего процесса.
 * @param curr_rank: ранг (индекс) текущего процесса.
 * @param comm: коммуникатор MPI.
 */
#define parallel_CurrRank(curr_rank, comm)                                     \
  {                                                                            \
    if (PARALLEL_MACROS_NEED_PRINT)                                            \
      std::cout << "parallel_CurrRank with args: comm: " << comm << std::endl; \
    parallel_CheckSuccess(MPI_Comm_rank(comm, &curr_rank), comm);              \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;       \
  }

/**
 * @brief Отправляет значение по сети MPI.
 * @param value: отправляемое значение.
 * @param datatype: тип данных значения.
 * @param to_rank: ранг получателя.
 * @param tag: тег сообщения.
 * @param comm: коммуникатор MPI.
 */
#define parallel_SendValue(value, datatype, to_rank, tag, comm)              \
  {                                                                          \
    if (PARALLEL_MACROS_NEED_PRINT)                                          \
      std::cout << "parallel_SendValue with args: value: " << value          \
                << "; datatype: " << datatype << "; to_rank: " << to_rank    \
                << "; tag: " << tag << "; comm: " << comm << std::endl;      \
    parallel_CheckSuccess(MPI_Send(&value, 1, datatype, to_rank, tag, comm), \
                          comm);                                             \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;     \
  }

/**
 * @brief Отправляет массив по сети MPI.
 * @param arr: массив, который нужно отправить.
 * @param arr_len: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param to_rank: ранг получателя.
 * @param tag: тег сообщения.
 * @param comm: коммуникатор MPI.
 */
#define parallel_SendArray(arr, arr_len, datatype, to_rank, tag, comm)       \
  {                                                                          \
    if (PARALLEL_MACROS_NEED_PRINT)                                          \
      std::cout << "parallel_SendArray with args: arr: " << arr              \
                << "; arr_len: " << arr_len << "; datatype: " << datatype    \
                << "; to_rank: " << to_rank << "; tag: " << tag              \
                << "; comm: " << comm << std::endl;                          \
    if (arr_len <= 0)                                                        \
      parallel_Error("parallel_SendArray: arr_len should be non-negative."); \
    parallel_CheckSuccess(                                                   \
        MPI_Send(arr, arr_len, datatype, to_rank, tag, comm), comm);         \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;     \
  }

/**
 * @brief Отправляет вектор по сети MPI.
 * @param arr: вектор, который нужно отправить.
 * @param datatype: тип данных элементов вектора.
 * @param to_rank: ранг получателя.
 * @param tag: тег сообщения.
 * @param comm: коммуникатор MPI.
 */
#define parallel_SendVector(vec, datatype, to_rank, tag, comm)             \
  {                                                                        \
    if (PARALLEL_MACROS_NEED_PRINT)                                        \
      std::cout << "parallel_SendVector with args: vec: " << vec           \
                << "; datatype: " << datatype << "; to_rank: " << to_rank  \
                << "; tag: " << tag << "; comm: " << comm << std::endl;    \
    if (vec.size() > INT_MAX)                                              \
      parallel_Error(                                                      \
          "parallel_SendVector: vector is too big (size > INT_MAX).");     \
    parallel_SendArray(vec.data(), static_cast<int>(vec.size()), datatype, \
                       to_rank, tag, comm);                                \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;   \
  }

/**
 * @brief Получает значение по сети MPI.
 * @param value: получаемое значение.
 * @param datatype: тип данных значения.
 * @param status: статус сообщения.
 * @param from_rank: ранг отправителя.
 * @param tag: тег сообщения.
 * @param comm: коммуникатор MPI.
 */
#define parallel_ReceiveValue(value, datatype, status, from_rank, tag, comm)  \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_ReceiveValue with args: value: " << value        \
                << "; datatype: " << datatype << "; from_rank: " << from_rank \
                << "; tag: " << tag << "; comm: " << comm << std::endl;       \
    parallel_CheckSuccess(                                                    \
        MPI_Recv(&value, 1, datatype, from_rank, tag, comm, &status), comm);  \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

/**
 * @brief Получает массив по сети MPI.
 * @param arr: массив, в который нужно получить данные.
 * @param arr_len: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param status: статус сообщения.
 * @param from_rank: ранг отправителя.
 * @param tag: тег сообщения.
 * @param comm: коммуникатор MPI.
 */
#define parallel_ReceiveArray(arr, arr_len, datatype, status, from_rank, tag, \
                              comm)                                           \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_ReceiveArray with args: arr: " << arr            \
                << "; arr_len: " << arr_len << "; datatype: " << datatype     \
                << "; from_rank: " << from_rank << "; tag: " << tag           \
                << "; comm: " << comm << std::endl;                           \
    if (arr_len <= 0)                                                         \
      parallel_Error(                                                         \
          "parallel_ReceiveArray: arr_len should be non-negative.");          \
    parallel_CheckSuccess(                                                    \
        MPI_Recv(arr, arr_len, datatype, from_rank, tag, comm, &status),      \
        comm);                                                                \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

/**
 * @brief Получает вектор по сети MPI.
 * @param vec: вектор, в который нужно получить данные.
 * @param datatype: тип данных элементов вектора.
 * @param status: статус сообщения.
 * @param from_rank: ранг отправителя.
 * @param tag: тег сообщения.
 * @param comm: коммуникатор MPI.
 */
#define parallel_ReceiveVector(vec, datatype, status, from_rank, tag, comm)    \
  {                                                                            \
    if (PARALLEL_MACROS_NEED_PRINT)                                            \
      std::cout << "parallel_ReceiveVectorValue with args: vec: " << vec       \
                << "; datatype: " << datatype << "; from_rank: " << from_rank  \
                << "; tag: " << tag << "; comm: " << comm << std::endl;        \
    if (vec.size() > INT_MAX)                                                  \
      parallel_Error(                                                          \
          "parallel_ReceiveVectorValue: vector is too big (size > INT_MAX)."); \
    parallel_ReceiveArray(vec.data(), static_cast<int>(vec.size()), datatype,  \
                          &status, from_rank, tag, comm);                      \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;       \
  }

/**
 * @brief Получает значение по сети MPI.
 * @param value: получаемое значение.
 * @param datatype: тип данных значения.
 * @param from_rank: ранг отправителя.
 * @param tag: тег сообщения.
 * @param comm: коммуникатор MPI.
 */
#define parallel_ReceiveIgnoreStatusValue(value, datatype, from_rank, tag,    \
                                          comm)                               \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_ReceiveIgnoreStatusValue with args: value: "     \
                << value << "; datatype: " << datatype                        \
                << "; from_rank: " << from_rank << "; tag: " << tag           \
                << "; comm: " << comm << std::endl;                           \
    parallel_CheckSuccess(MPI_Recv(&value, 1, datatype, from_rank, tag, comm, \
                                   MPI_STATUS_IGNORE),                        \
                          comm);                                              \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

/**
 * @brief Получает массив по сети MPI.
 * @param arr: массив, в который нужно получить данные.
 * @param arr_len: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param from_rank: ранг отправителя.
 * @param tag: тег сообщения.
 * @param comm: коммуникатор MPI.
 */
#define parallel_ReceiveIgnoreStatusArray(arr, arr_len, datatype, from_rank,   \
                                          tag, comm)                           \
  {                                                                            \
    if (PARALLEL_MACROS_NEED_PRINT)                                            \
      std::cout << "parallel_ReceiveIgnoreStatusArray with args: arr: " << arr \
                << "; arr_len: " << arr_len << "; datatype: " << datatype      \
                << "; from_rank: " << from_rank << "; tag: " << tag            \
                << "; comm: " << comm << std::endl;                            \
    if (arr_len <= 0)                                                          \
      parallel_Error(                                                          \
          "parallel_ReceiveIgnoreStatusArray: arr_len should be "              \
          "non-negative.");                                                    \
    parallel_CheckSuccess(MPI_Recv(arr, arr_len, datatype, from_rank, tag,     \
                                   comm, MPI_STATUS_IGNORE),                   \
                          comm);                                               \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;       \
  }

/**
 * @brief Получает вектор по сети MPI.
 * @param vec: вектор, в который нужно получить данные.
 * @param datatype: тип данных элементов вектора.
 * @param from_rank: ранг отправителя.
 * @param tag: тег сообщения.
 * @param comm: коммуникатор MPI.
 */
#define parallel_ReceiveIgnoreStatusVector(vec, datatype, from_rank, tag,     \
                                           comm)                              \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_ReceiveIgnoreStatusVector with args: vec: "      \
                << vec << "; datatype: " << datatype                          \
                << "; from_rank: " << from_rank << "; tag: " << tag           \
                << "; comm: " << comm << std::endl;                           \
    if (vec.size() > INT_MAX)                                                 \
      parallel_Error(                                                         \
          "parallel_ReceiveIgnoreStatusVector: vector is too big (size > "    \
          "INT_MAX).");                                                       \
    parallel_ReceiveIgnoreStatusArray(vec.data(),                             \
                                      static_cast<int>(vec.size()), datatype, \
                                      from_rank, tag, comm);                  \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

/**
 * @brief Рассылает значение от процесса с указанным рангом по сети MPI.
 * @param value: рассылаемое значение.
 * @param datatype: тип данных значения.
 * @param from_rank: ранг процесса рассылки.
 * @param comm: коммуникатор MPI.
 */
#define parallel_BroadcastValue(value, datatype, from_rank, comm)             \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_BroadcastValue with args: value: " << value      \
                << "; datatype: " << datatype << "; from_rank: " << from_rank \
                << "; comm: " << comm << std::endl;                           \
    parallel_CheckSuccess(MPI_Bcast(&value, 1, datatype, from_rank, comm),    \
                          comm);                                              \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

/**
 * @brief Рассылает массив от процесса с указанным рангом по сети MPI.
 * @param arr: рассылаемый массив.
 * @param arr_len: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param from_rank: ранг процесса рассылки.
 * @param comm: коммуникатор MPI.
 */
#define parallel_BroadcastArray(arr, arr_len, datatype, from_rank, comm)      \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_BroadcastArray with args: arr: " << arr          \
                << "; arr_len: " << arr_len << "; datatype: " << datatype     \
                << "; from_rank: " << from_rank << "; comm: " << comm         \
                << std::endl;                                                 \
    if (arr_len <= 0)                                                         \
      parallel_Error(                                                         \
          "parallel_BroadcastArray: arr_len should be non-negative.");        \
    parallel_CheckSuccess(MPI_Bcast(arr, arr_len, datatype, from_rank, comm), \
                          comm);                                              \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

/**
 * @brief Рассылает вектор от процесса с указанным рангом по сети MPI.
 * @param vec: рассылаемый вектор.
 * @param datatype: тип данных элементов вектора.
 * @param from_rank: ранг процесса рассылки.
 * @param comm: коммуникатор MPI.
 */
#define parallel_BroadcastVector(vec, datatype, from_rank, comm)              \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_BroadcastVector with args: vec: " << vec         \
                << "; datatype: " << datatype << "; from_rank: " << from_rank \
                << "; comm: " << comm << std::endl;                           \
    if (vec.size() > INT_MAX)                                                 \
      parallel_Error(                                                         \
          "parallel_BroadcastVector: vector is too big (size > INT_MAX).");   \
    parallel_BroadcastArray(vec.data(), static_cast<int>(vec.size()),         \
                            datatype, from_rank, comm);                       \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

/**
 * @brief Выполняет операцию над значением и отправляет результат на
 * указанный процесс в сети MPI.
 * @param from_value: исходное значение, над которым выполняется операция.
 * @param to_value: значение, куда будет записан результат операции.
 * @param datatype: тип данных значения.
 * @param op: операция MPI, которая будет выполнена.
 * @param to_rank: ранг процесса результата.
 * @param comm: коммуникатор MPI.
 */
#define parallel_OperationValue(from_value, to_value, datatype, op, to_rank, \
                                comm)                                        \
  {                                                                          \
    if (PARALLEL_MACROS_NEED_PRINT)                                          \
      std::cout << "parallel_OperationValue with args: from_value: "         \
                << from_value << "; to_value: " << to_value                  \
                << "; datatype: " << datatype << "; op: " << op              \
                << "; to_rank: " << to_rank << "; comm: " << comm            \
                << std::endl;                                                \
    parallel_CheckSuccess(                                                   \
        MPI_Reduce(&from_value, &to_value, 1, datatype, op, to_rank, comm)); \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;     \
  }

/**
 * @brief Выполняет операцию над массивом значений и отправляет результат на
 * указанный процесс в сети MPI.
 * @param from_arr: исходный массив значений, над которым выполняется операция.
 * @param to_arr: массив, куда будет записан результат операции.
 * @param arr_len: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param op: операция MPI, которая будет выполнена.
 * @param to_rank: ранг процесса результата.
 * @param comm: коммуникатор MPI.
 */
#define parallel_OperationArray(from_arr, to_arr, arr_len, datatype, op,       \
                                to_rank, comm)                                 \
  {                                                                            \
    if (PARALLEL_MACROS_NEED_PRINT)                                            \
      std::cout << "parallel_OperationArray with args: from_arr: " << from_arr \
                << "; to_arr: " << to_arr << "; arr_len: " << arr_len          \
                << "; datatype: " << datatype << "; op: " << op                \
                << "; to_rank: " << to_rank << "; comm: " << comm              \
                << std::endl;                                                  \
    if (arr_len <= 0)                                                          \
      parallel_Error(                                                          \
          "parallel_OperationArray: arr_len should be non-negative.");         \
    parallel_CheckSuccess(                                                     \
        MPI_Reduce(from_arr, to_arr, arr_len, datatype, op, to_rank, comm));   \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;       \
  }

/**
 * @brief Выполняет операцию над вектором значений и отправляет результат на
 * указанный процесс в сети MPI.
 * @param from_vec: исходный вектор значений, над которым выполняется операция.
 * @param to_vec: вектор, куда будет записан результат операции.
 * @param datatype: тип данных в векторе.
 * @param op: операция MPI, которая будет выполнена.
 * @param to_rank: ранг процесса результата.
 * @param comm: коммуникатор MPI.
 */
#define parallel_OperationVector(from_vec, to_vec, datatype, op, to_rank,   \
                                 comm)                                      \
  {                                                                         \
    if (PARALLEL_MACROS_NEED_PRINT)                                         \
      std::cout << "parallel_OperationVector with args: from_vec: "         \
                << from_vec << "; to_vec: " << to_vec                       \
                << "; datatype: " << datatype << "; op: " << op             \
                << "; to_rank: " << to_rank << "; comm: " << comm           \
                << std::endl;                                               \
    if (from_vec.size() > INT_MAX || to_vec.size() > INT_MAX)               \
      parallel_Error(                                                       \
          "parallel_OperationVector: vector is too big (size > INT_MAX)."); \
    parallel_OperationArray(from_vec.data(), to_vec.data(),                 \
                            Min(static_cast<int>(from_vec.size()),          \
                                static_cast<int>(to_vec.size())),           \
                            datatype, op, to_rank, comm);                   \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;    \
  }

/**
 * @brief Собирать значения от всех процессов в сети MPI в одном процессе.
 * @param from_value: значение, которое будет отправлено от текущего процесса.
 * @param from_value_datatype: тип данных `from_value`.
 * @param to_value: ссылка на значение, куда будет записан результат сбора на
 * процессе `to_rank`.
 * @param to_value_datatype: тип данных `to_value`.
 * @param to_rank: ранг процесса результата.
 * @param comm: коммуникатор MPI.
 */
#define parallel_GatherValue(from_value, from_value_datatype, to_value,        \
                             to_value_datatype, to_rank, comm)                 \
  {                                                                            \
    if (PARALLEL_MACROS_NEED_PRINT)                                            \
      std::cout << "parallel_GatherValue with args: from_value: "              \
                << from_value                                                  \
                << "; from_value_datatype: " << from_value_datatype            \
                << "; to_value: " << to_value                                  \
                << "; to_value_datatype: " << to_value_datatype                \
                << "; to_rank: " << to_rank << "; comm: " << comm              \
                << std::endl;                                                  \
    parallel_CheckSuccess(MPI_Gather(&from_value, 1, from_value_datatype,      \
                                     &to_value, 1, to_value_datatype, to_rank, \
                                     comm));                                   \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;       \
  }

/**
 * @brief Собирает массивы от всех процессов в сети MPI в одном процессе.
 * @param from_arr: массив, который будет отправлен от текущего процесса.
 * @param from_arr_len: количество элементов в массиве `from_arr`.
 * @param from_arr_datatype: тип данных элементов массива `from_arr`.
 * @param to_arr: массив, куда будет записан результат сбора на процессе
 * `to_rank`.
 * @param to_arr_len: количество элементов в массиве `to_arr`.
 * @param to_arr_datatype: тип данных элементов массива `to_arr`.
 * @param to_rank: ранг процесса результата.
 * @param comm: коммуникатор MPI.
 */
#define parallel_GatherArray(from_arr, from_arr_len, from_arr_datatype,        \
                             to_arr, to_arr_len, to_arr_datatype, to_rank,     \
                             comm)                                             \
  {                                                                            \
    if (PARALLEL_MACROS_NEED_PRINT)                                            \
      std::cout << "parallel_GatherArray with args: from_arr: "                \
                << from_arr_len << "; from_arr_len: " << from_arr_len          \
                << "; from_arr_datatype: " << from_arr_datatype                \
                << "; to_arr: " << to_arr << "; to_arr_len: " << to_arr_len    \
                << "; to_arr_datatype: " << to_arr_datatype                    \
                << "; to_rank: " << to_rank << "; comm: " << comm              \
                << std::endl;                                                  \
    if (from_arr_len < 0 || to_arr_len < 0)                                    \
      parallel_Error("parallel_GatherArray: arr_len should be non-negative."); \
    parallel_CheckSuccess(MPI_Gather(from_arr, from_arr_len,                   \
                                     from_arr_datatype, to_arr, to_arr_len,    \
                                     to_arr_datatype, to_rank, comm));         \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;       \
  }

/**
 * @brief Собирает векторы от всех процессов в сети MPI в одном процессе.
 * @param from_vec: вектор, который будет отправлен от текущего процесса.
 * @param from_vec_datatype: тип данных элементов вектора `from_arr`.
 * @param to_vec: вектор, куда будет записан результат сбора на процессе
 * `to_rank`.
 * @param to_vec_datatype: тип данных элементов вектора `to_arr`.
 * @param to_rank: ранг процесса результата.
 * @param comm: коммуникатор MPI.
 */
#define parallel_GatherVector(from_vec, from_vec_datatype, to_vec,           \
                              to_vec_datatype, to_rank, comm)                \
  {                                                                          \
    if (PARALLEL_MACROS_NEED_PRINT)                                          \
      std::cout << "parallel_GatherVector with args: from_vec: " << from_vec \
                << "; from_vec_datatype: " << from_vec_datatype              \
                << "; to_vec: " << to_vec                                    \
                << "; to_vec_datatype: " << to_vec_datatype                  \
                << "; to_rank: " << to_rank << "; comm: " << comm            \
                << std::endl;                                                \
    if (from_vec.size() > INT_MAX || to_vec.size() > INT_MAX)                \
      parallel_Error(                                                        \
          "parallel_GatherVector: vector is too big (size > INT_MAX).");     \
    parallel_GatherArray(from_vec.data(), static_cast<int>(from_vec.size()), \
                         from_vec_datatype, to_vec.data(),                   \
                         static_cast<int>(to_vec.size()), to_vec_datatype,   \
                         to_rank, comm);                                     \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;     \
  }

/**
 * @brief Собирает массивы от всех процессов в сети MPI в одном процессе с
 * различными размерами для каждого процесса.
 * @param from_arr: массив, который будет отправлен от текущего процесса.
 * @param from_arr_datatype: тип данных элементов массива `from_arr`.
 * @param to_arr: массив, куда будет записан результат сбора на процессе
 * `to_rank`.
 * @param to_arr_datatype: тип данных элементов массива `to_arr`.
 * @param to_arr_counts: количество значений, которое будет получено от каждого
 * процесса.
 * @param displacements: смещения в массиве `to_arr` для каждого
 * процесса.
 * @param arr_len: количество элементов в массиве `from_arr` и `to_arr`.
 * @param to_rank: ранг процесса результата.
 * @param comm: коммуникатор MPI.
 */
#define parallel_GatherVariousArray(from_arr, from_arr_datatype, to_arr,    \
                                    to_arr_datatype, to_arr_counts,         \
                                    displacements, arr_len, to_rank, comm)  \
  {                                                                         \
    if (PARALLEL_MACROS_NEED_PRINT)                                         \
      std::cout << "parallel_GatherVariousArray with args: from_arr: "      \
                << from_arr << "; from_arr_datatype: " << from_arr_datatype \
                << "; to_arr: " << to_arr                                   \
                << "; to_arr_datatype: " << to_arr_datatype                 \
                << "; to_arr_counts: " << to_arr_counts                     \
                << "; displacements: " << displacements                     \
                << "; arr_len: " << arr_len << "; to_rank: " << to_rank     \
                << "; comm: " << comm << std::endl;                         \
    if (arr_len < 0)                                                        \
      parallel_Error(                                                       \
          "parallel_GatherVariousArray: arr_len should be non-negative.");  \
    parallel_CheckSuccess(MPI_Gatherv(from_arr, arr_len, from_arr_datatype, \
                                      to_arr, to_arr_counts, displacements, \
                                      to_arr_datatype, to_rank, comm));     \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;    \
  }

/**
 * @brief Собирает векторы от всех процессов в сети MPI в одном процессе с
 * различными размерами для каждого процесса.
 * @param from_vec: вектор, который будет отправлен от текущего процесса.
 * @param from_vec_datatype: тип данных элементов вектора `from_arr`.
 * @param to_vec: вектор, куда будет записан результат сбора на процессе
 * `to_rank`.
 * @param to_vec_datatype: тип данных элементов вектора `to_arr`.
 * @param to_vec_counts: количество значений, которое будет получено от каждого
 * процесса.
 * @param displacements: смещения в векторе `to_vec` для каждого
 * процесса.
 * @param to_rank: ранг процесса результата.
 * @param comm: коммуникатор MPI.
 */
#define parallel_GatherVariousVector(from_vec, from_vec_datatype, to_vec,    \
                                     to_vec_datatype, to_vec_counts,         \
                                     displacements, to_rank, comm)           \
  {                                                                          \
    if (PARALLEL_MACROS_NEED_PRINT)                                          \
      std::cout << "parallel_GatherVariousVector with args: from_vec: "      \
                << from_vec << "; from_vec_datatype: " << from_vec_datatype  \
                << "; to_vec: " << to_vec                                    \
                << "; to_vec_datatype: " << to_vec_datatype                  \
                << "; to_vec_counts: " << to_vec_counts                      \
                << "; displacements: " << displacements                      \
                << "; to_rank: " << to_rank << "; comm: " << comm            \
                << std::endl;                                                \
    if (from_vec.size() > INT_MAX || to_vec.size() > INT_MAX ||              \
        to_vec_counts.size() > INT_MAX || displacements.size() > INT_MAX)    \
      parallel_Error(                                                        \
          "parallel_GatherVariousVector: vector is too big (size > "         \
          "INT_MAX).");                                                      \
    parallel_GatherVariousArray(from_vec.data(), from_vec_datatype,          \
                                to_vec.data(), to_vec_datatype,              \
                                to_vec_counts.data(), displacements.data(),  \
                                Min(static_cast<int>(from_vec.size()),       \
                                    static_cast<int>(to_vec.size()),         \
                                    static_cast<int>(to_vec_counts.size()),  \
                                    static_cast<int>(displacements.size())), \
                                to_rank, comm);                              \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;     \
  }

#else

/// @brief Аварийно завершает работу со средой MPI.
#define parallel_Abort() MPI_Abort(MPI_COMM_WORLD, 1)

/**
 * @brief Завершает выполнение процесса MPI, если состояние не равно
 * MPI_SUCCESS.
 * @param state: состояние выполнения MPI.
 */
#define parallel_CheckSuccess(state)         \
  {                                          \
    int st = state;                          \
    if (st != MPI_SUCCESS) parallel_Abort(); \
  }

/**
 * @brief Аварийно завершает работу со средой MPI, выводя ошибку в поток
 * std::cerr.
 * @param error_text: текст ошибки.
 */
#define parallel_Error(error_text)                                          \
  {                                                                         \
    std::cerr << "Error! Aborting because of: " << error_text << std::endl; \
    parallel_Abort();                                                       \
  }

/**
 * @brief Инициализирует среду MPI.
 * @param argc: количество аргументов командной строки.
 * @param argv: массив аргументов командной строки.
 */
#define parallel_Init(argc, argv)                                        \
  {                                                                      \
    if (PARALLEL_MACROS_NEED_PRINT)                                      \
      std::cout << "parallel_Init with args: argc: " << argc             \
                << "; argv: " << argv << std::endl;                      \
    parallel_CheckSuccess(MPI_Init(&argc, &argv));                       \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl; \
  }

/// @brief Завершает работу со средой MPI.
#define parallel_Finalize()                                              \
  {                                                                      \
    parallel_CheckSuccess(MPI_Finalize());                               \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl; \
  }

/**
 * @brief Считает количество процессов в коммуникаторе.
 * @param ranks_amount: количество процессов в коммуникаторе.
 */
#define parallel_RanksAmount(ranks_amount)                                    \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_RanksAmount with args: comm: " << MPI_COMM_WORLD \
                << std::endl;                                                 \
    parallel_CheckSuccess(MPI_Comm_size(MPI_COMM_WORLD, &ranks_amount));      \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

/**
 * @brief Считает ранг (индекс) текущего процесса.
 * @param curr_rank: ранг (индекс) текущего процесса.
 */
#define parallel_CurrRank(curr_rank)                                       \
  {                                                                        \
    if (PARALLEL_MACROS_NEED_PRINT)                                        \
      std::cout << "parallel_CurrRank with args: comm: " << MPI_COMM_WORLD \
                << std::endl;                                              \
    parallel_CheckSuccess(MPI_Comm_rank(MPI_COMM_WORLD, &curr_rank));      \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;   \
  }

/**
 * @brief Отправляет значение по сети MPI.
 * @param value: отправляемое значение.
 * @param datatype: тип данных значения.
 * @param to_rank: ранг получателя.
 */
#define parallel_SendValue(value, datatype, to_rank)                        \
  {                                                                         \
    if (PARALLEL_MACROS_NEED_PRINT)                                         \
      std::cout << "parallel_SendValue with args: value: " << value         \
                << "; datatype: " << datatype << "; to_rank: " << to_rank   \
                << "; tag: " << PARALLEL_STANDARD_TAG                       \
                << "; comm: " << MPI_COMM_WORLD << std::endl;               \
    parallel_CheckSuccess(MPI_Send(&value, 1, datatype, to_rank,            \
                                   PARALLEL_STANDARD_TAG, MPI_COMM_WORLD)); \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;    \
  }

/**
 * @brief Отправляет массив по сети MPI.
 * @param arr: массив, который нужно отправить.
 * @param arr_len: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param to_rank: ранг получателя.
 */
#define parallel_SendArray(arr, arr_len, datatype, to_rank)                 \
  {                                                                         \
    if (PARALLEL_MACROS_NEED_PRINT)                                         \
      std::cout << "parallel_SendArray with args: arr: " << arr             \
                << "; arr_len: " << arr_len << "; datatype: " << datatype   \
                << "; to_rank: " << to_rank                                 \
                << "; tag: " << PARALLEL_STANDARD_TAG                       \
                << "; comm: " << MPI_COMM_WORLD << std::endl;               \
    if (arr_len <= 0)                                                       \
      parallel_Error("parallel::Send: arr_len should be non-negative.");    \
    parallel_CheckSuccess(MPI_Send(arr, arr_len, datatype, to_rank,         \
                                   PARALLEL_STANDARD_TAG, MPI_COMM_WORLD)); \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;    \
  }

/**
 * @brief Отправляет вектор по сети MPI.
 * @param arr: вектор, который нужно отправить.
 * @param datatype: тип данных элементов вектора.
 * @param to_rank: ранг получателя.
 */
#define parallel_SendVector(vec, datatype, to_rank)                          \
  {                                                                          \
    if (PARALLEL_MACROS_NEED_PRINT)                                          \
      std::cout << "parallel_SendVector with args: vec: " << vec             \
                << "; datatype: " << datatype << "; to_rank: " << to_rank    \
                << "; tag: " << PARALLEL_STANDARD_TAG                        \
                << "; comm: " << MPI_COMM_WORLD << std::endl;                \
    if (vec.size() > INT_MAX)                                                \
      parallel_Error("parallel::Send: vector is too big (size > INT_MAX)."); \
    parallel_SendArray(vec.data(), static_cast<int>(vec.size()), datatype,   \
                       to_rank);                                             \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;     \
  }

/**
 * @brief Получает значение по сети MPI.
 * @param value: получаемое значение.
 * @param datatype: тип данных значения.
 * @param status: статус сообщения.
 * @param from_rank: ранг отправителя. По умолчанию MPI_ANY_SOURCE.
 */
#define parallel_ReceiveValue(value, datatype, status, from_rank)             \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_ReceiveValue with args: value: " << value        \
                << "; datatype: " << datatype << "; from_rank: " << from_rank \
                << "; tag: " << PARALLEL_STANDARD_TAG                         \
                << "; comm: " << MPI_COMM_WORLD << std::endl;                 \
    parallel_CheckSuccess(MPI_Recv(&value, 1, datatype, from_rank,            \
                                   PARALLEL_STANDARD_TAG, MPI_COMM_WORLD,     \
                                   &status));                                 \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

/**
 * @brief Получает массив по сети MPI.
 * @param arr: массив, в который нужно получить данные.
 * @param arr_len: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param status: статус сообщения.
 * @param from_rank: ранг отправителя.
 */
#define parallel_ReceiveArray(arr, arr_len, datatype, status, from_rank)    \
  {                                                                         \
    if (PARALLEL_MACROS_NEED_PRINT)                                         \
      std::cout << "parallel_ReceiveArray with args: arr: " << arr          \
                << "; arr_len: " << arr_len << "; datatype: " << datatype   \
                << "; from_rank: " << from_rank                             \
                << "; tag: " << PARALLEL_STANDARD_TAG                       \
                << "; comm: " << MPI_COMM_WORLD << std::endl;               \
    if (arr_len <= 0)                                                       \
      parallel_Error("parallel::Receive: arr_len should be non-negative."); \
    parallel_CheckSuccess(MPI_Recv(arr, arr_len, datatype, from_rank,       \
                                   PARALLEL_STANDARD_TAG, MPI_COMM_WORLD,   \
                                   &status));                               \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;    \
  }

/**
 * @brief Получает вектор по сети MPI.
 * @param vec: вектор, в который нужно получить данные.
 * @param datatype: тип данных элементов вектора.
 * @param status: статус сообщения.
 * @param from_rank: ранг отправителя.
 */
#define parallel_ReceiveVector(vec, datatype, status, from_rank)              \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_ReceiveVector with args: vec: " << vec           \
                << "; datatype: " << datatype << "; from_rank: " << from_rank \
                << "; tag: " << PARALLEL_STANDARD_TAG                         \
                << "; comm: " << MPI_COMM_WORLD << std::endl;                 \
    if (vec.size() > INT_MAX)                                                 \
      parallel_Error(                                                         \
          "parallel::Receive: vector is too big (size > INT_MAX).");          \
    parallel_ReceiveArray(vec.data(), static_cast<int>(vec.size()), datatype, \
                          &status, from_rank);                                \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

/**
 * @brief Получает значение по сети MPI.
 * @param value: получаемое значение.
 * @param datatype: тип данных значения.
 * @param from_rank: ранг отправителя.
 */
#define parallel_ReceiveIgnoreStatusValue(value, datatype, from_rank)     \
  {                                                                       \
    if (PARALLEL_MACROS_NEED_PRINT)                                       \
      std::cout << "parallel_ReceiveIgnoreStatusValue with args: value: " \
                << value << "; datatype: " << datatype                    \
                << "; from_rank: " << from_rank                           \
                << "; tag: " << PARALLEL_STANDARD_TAG                     \
                << "; comm: " << MPI_COMM_WORLD << std::endl;             \
    parallel_CheckSuccess(MPI_Recv(&value, 1, datatype, from_rank,        \
                                   PARALLEL_STANDARD_TAG, MPI_COMM_WORLD, \
                                   MPI_STATUS_IGNORE));                   \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;  \
  }

/**
 * @brief Получает массив по сети MPI.
 * @param arr: массив, в который нужно получить данные.
 * @param arr_len: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param from_rank: ранг отправителя.
 */
#define parallel_ReceiveIgnoreStatusArray(arr, arr_len, datatype, from_rank)   \
  {                                                                            \
    if (PARALLEL_MACROS_NEED_PRINT)                                            \
      std::cout << "parallel_ReceiveIgnoreStatusArray with args: arr: " << arr \
                << "; arr_len: " << arr_len << "; datatype: " << datatype      \
                << "; from_rank: " << from_rank                                \
                << "; tag: " << PARALLEL_STANDARD_TAG                          \
                << "; comm: " << MPI_COMM_WORLD << std::endl;                  \
    if (arr_len <= 0)                                                          \
      parallel_Error(                                                          \
          "parallel_ReceiveIgnoreStatusArray: arr_len should be "              \
          "non-negative.");                                                    \
    parallel_CheckSuccess(MPI_Recv(arr, arr_len, datatype, from_rank,          \
                                   PARALLEL_STANDARD_TAG, MPI_COMM_WORLD,      \
                                   MPI_STATUS_IGNORE));                        \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;       \
  }

/**
 * @brief Получает вектор по сети MPI.
 * @param vec: вектор, в который нужно получить данные.
 * @param datatype: тип данных элементов вектора.
 * @param from_rank: ранг отправителя.
 */
#define parallel_ReceiveIgnoreStatusVector(vec, datatype, from_rank)       \
  {                                                                        \
    if (PARALLEL_MACROS_NEED_PRINT)                                        \
      std::cout << "parallel_ReceiveIgnoreStatusVector with args: vec: "   \
                << vec << "; datatype: " << datatype                       \
                << "; from_rank: " << from_rank                            \
                << "; tag: " << PARALLEL_STANDARD_TAG                      \
                << "; comm: " << MPI_COMM_WORLD << std::endl;              \
    if (vec.size() > INT_MAX)                                              \
      parallel_Error(                                                      \
          "parallel_ReceiveIgnoreStatusVector: vector is too big (size > " \
          "INT_MAX).");                                                    \
    parallel_ReceiveIgnoreStatusArray(                                     \
        vec.data(), static_cast<int>(vec.size()), datatype, from_rank);    \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;   \
  }

/**
 * @brief Рассылает значение от процесса с рангом 0 по сети MPI.
 * @param value: рассылаемое значение.
 * @param datatype: тип данных значения.
 */
#define parallel_BroadcastValue(value, datatype)                              \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_BroadcastValue with args: value: " << value      \
                << "; datatype: " << datatype << "; from_rank: " << 0         \
                << "; comm: " << MPI_COMM_WORLD << std::endl;                 \
    parallel_CheckSuccess(MPI_Bcast(&value, 1, datatype, 0, MPI_COMM_WORLD)); \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

/**
 * @brief Рассылает массив от процесса с рангом 0 по сети MPI.
 * @param arr: рассылаемый массив.
 * @param arr_len: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 */
#define parallel_BroadcastArray(arr, arr_len, datatype)                   \
  {                                                                       \
    if (PARALLEL_MACROS_NEED_PRINT)                                       \
      std::cout << "parallel_BroadcastArray with args: arr: " << arr      \
                << "; arr_len: " << arr_len << "; datatype: " << datatype \
                << "; from_rank: " << 0 << "; comm: " << MPI_COMM_WORLD   \
                << std::endl;                                             \
    if (arr_len <= 0)                                                     \
      parallel_Error(                                                     \
          "parallel_BroadcastArray: arr_len should be non-negative.");    \
    parallel_CheckSuccess(                                                \
        MPI_Bcast(arr, arr_len, datatype, 0, MPI_COMM_WORLD));            \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;  \
  }

/**
 * @brief Рассылает вектор от процесса с рангом 0 по сети MPI.
 * @param vec: рассылаемый вектор.
 * @param datatype: тип данных элементов вектора.
 */
#define parallel_BroadcastVector(vec, datatype)                             \
  {                                                                         \
    if (PARALLEL_MACROS_NEED_PRINT)                                         \
      std::cout << "parallel_BroadcastVector with args: vec: " << vec       \
                << "; datatype: " << datatype << "; from_rank: " << 0       \
                << "; comm: " << MPI_COMM_WORLD << std::endl;               \
    if (vec.size() > INT_MAX)                                               \
      parallel_Error(                                                       \
          "parallel_BroadcastVector: vector is too big (size > INT_MAX)."); \
    parallel_BroadcastArray(vec.data(), static_cast<int>(vec.size()),       \
                            datatype);                                      \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;    \
  }

/**
 * @brief Выполняет операцию над значением и отправляет результат на
 * процесс с рангом 0 в сети MPI.
 * @param from_value: исходное значение, над которым выполняется операция.
 * @param to_value: значение, куда будет записан результат операции.
 * @param datatype: тип данных значения.
 * @param op: операция MPI, которая будет выполнена.
 */
#define parallel_OperationValue(from_value, to_value, datatype, op)           \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_OperationValue with args: from_value: "          \
                << from_value << "; to_value: " << to_value                   \
                << "; datatype: " << datatype << "; op: " << op               \
                << "; to_rank: " << 0 << "; comm: " << MPI_COMM_WORLD         \
                << std::endl;                                                 \
    parallel_CheckSuccess(MPI_Reduce(&from_value, &to_value, 1, datatype, op, \
                                     0, MPI_COMM_WORLD));                     \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

/**
 * @brief Выполняет операцию над массивом значений и отправляет результат на
 * процесс с рангом 0 в сети MPI.
 * @param from_arr: исходный массив значений, над которым выполняется операция.
 * @param to_arr: массив, куда будет записан результат операции.
 * @param arr_len: количество элементов в массиве.
 * @param datatype: тип данных элементов массива.
 * @param op: операция MPI, которая будет выполнена.
 */
#define parallel_OperationArray(from_arr, to_arr, arr_len, datatype, op)       \
  {                                                                            \
    if (PARALLEL_MACROS_NEED_PRINT)                                            \
      std::cout << "parallel_OperationArray with args: from_arr: " << from_arr \
                << "; to_arr: " << to_arr << "; arr_len: " << arr_len          \
                << "; datatype: " << datatype << "; op: " << op                \
                << "; to_rank: " << 0 << "; comm: " << MPI_COMM_WORLD          \
                << std::endl;                                                  \
    if (arr_len <= 0)                                                          \
      parallel_Error(                                                          \
          "parallel_OperationArray: arr_len should be non-negative.");         \
    parallel_CheckSuccess(MPI_Reduce(from_arr, to_arr, arr_len, datatype, op,  \
                                     0, MPI_COMM_WORLD));                      \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;       \
  }

/**
 * @brief Выполняет операцию над вектором значений и отправляет результат на
 * процесс с рангом 0 в сети MPI.
 * @param from_vec: исходный вектор значений, над которым выполняется операция.
 * @param to_vec: вектор, куда будет записан результат операции.
 * @param datatype: тип данных в векторе.
 * @param op: операция MPI, которая будет выполнена.
 */
#define parallel_OperationVector(from_vec, to_vec, datatype, op)            \
  {                                                                         \
    if (PARALLEL_MACROS_NEED_PRINT)                                         \
      std::cout << "parallel_OperationVector with args: from_vec: "         \
                << from_vec << "; to_vec: " << to_vec                       \
                << "; datatype: " << datatype << "; op: " << op             \
                << "; to_rank: " << 0 << "; comm: " << MPI_COMM_WORLD       \
                << std::endl;                                               \
    if (from_vec.size() > INT_MAX || to_vec.size() > INT_MAX)               \
      parallel_Error(                                                       \
          "parallel_OperationVector: vector is too big (size > INT_MAX)."); \
    parallel_OperationArray(from_vec.data(), to_vec.data(),                 \
                            Min(static_cast<int>(from_vec.size()),          \
                                static_cast<int>(to_vec.size())),           \
                            datatype, op, 0, MPI_COMM_WORLD);               \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;    \
  }

/**
 * @brief Собирать значения от всех процессов в сети MPI в одном процессе.
 * @param from_value: значение, которое будет отправлено от текущего процесса.
 * @param from_value_datatype: тип данных `from_value`.
 * @param to_value: ссылка на значение, куда будет записан результат сбора на
 * процессе `to_rank`.
 * @param to_value_datatype: тип данных `to_value`.
 */
#define parallel_GatherValue(from_value, from_value_datatype, to_value,   \
                             to_value_datatype)                           \
  {                                                                       \
    if (PARALLEL_MACROS_NEED_PRINT)                                       \
      std::cout << "parallel_GatherValue with args: from_value: "         \
                << from_value                                             \
                << "; from_value_datatype: " << from_value_datatype       \
                << "; to_value: " << to_value                             \
                << "; to_value_datatype: " << to_value_datatype           \
                << "; to_rank: " << 0 << "; comm: " << MPI_COMM_WORLD     \
                << std::endl;                                             \
    parallel_CheckSuccess(MPI_Gather(&from_value, 1, from_value_datatype, \
                                     &to_value, 1, to_value_datatype, 0,  \
                                     MPI_COMM_WORLD));                    \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;  \
  }

/**
 * @brief Собирает массивы от всех процессов в сети MPI в одном процессе.
 * @param from_arr: массив, который будет отправлен от текущего процесса.
 * @param from_arr_len: количество элементов в массиве `from_arr`.
 * @param from_arr_datatype: тип данных элементов массива `from_arr`.
 * @param to_arr: массив, куда будет записан результат сбора на процессе
 * `to_rank`.
 * @param to_arr_len: количество элементов в массиве `to_arr`.
 * @param to_arr_datatype: тип данных элементов массива `to_arr`.
 */
#define parallel_GatherArray(from_arr, from_arr_len, from_arr_datatype,        \
                             to_arr, to_arr_len, to_arr_datatype)              \
  {                                                                            \
    if (PARALLEL_MACROS_NEED_PRINT)                                            \
      std::cout << "parallel_GatherArray with args: from_arr: "                \
                << from_arr_len << "; from_arr_len: " << from_arr_len          \
                << "; from_arr_datatype: " << from_arr_datatype                \
                << "; to_arr: " << to_arr << "; to_arr_len: " << to_arr_len    \
                << "; to_arr_datatype: " << to_arr_datatype                    \
                << "; to_rank: " << 0 << "; comm: " << MPI_COMM_WORLD          \
                << std::endl;                                                  \
    if (from_arr_len < 0 || to_arr_len < 0)                                    \
      parallel_Error("parallel_GatherArray: arr_len should be non-negative."); \
    parallel_CheckSuccess(MPI_Gather(from_arr, from_arr_len,                   \
                                     from_arr_datatype, to_arr, to_arr_len,    \
                                     to_arr_datatype, 0, MPI_COMM_WORLD));     \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;       \
  }

/**
 * @brief Собирает векторы от всех процессов в сети MPI в одном процессе.
 * @param from_vec: вектор, который будет отправлен от текущего процесса.
 * @param from_vec_datatype: тип данных элементов вектора `from_arr`.
 * @param to_vec: вектор, куда будет записан результат сбора на процессе
 * `to_rank`.
 * @param to_vec_datatype: тип данных элементов вектора `to_arr`.
 */
#define parallel_GatherVector(from_vec, from_vec_datatype, to_vec,           \
                              to_vec_datatype)                               \
  {                                                                          \
    if (PARALLEL_MACROS_NEED_PRINT)                                          \
      std::cout << "parallel_GatherVector with args: from_vec: " << from_vec \
                << "; from_vec_datatype: " << from_vec_datatype              \
                << "; to_vec: " << to_vec                                    \
                << "; to_vec_datatype: " << to_vec_datatype                  \
                << "; to_rank: " << 0 << "; comm: " << MPI_COMM_WORLD        \
                << std::endl;                                                \
    if (from_vec.size() > INT_MAX || to_vec.size() > INT_MAX)                \
      parallel_Error(                                                        \
          "parallel_GatherVector: vector is too big (size > INT_MAX).");     \
    parallel_GatherArray(from_vec.data(), static_cast<int>(from_vec.size()), \
                         from_vec_datatype, to_vec.data(),                   \
                         static_cast<int>(to_vec.size()), to_vec_datatype);  \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;     \
  }

/**
 * @brief Собирает массивы от всех процессов в сети MPI в одном процессе с
 * различными размерами для каждого процесса.
 * @param from_arr: массив, который будет отправлен от текущего процесса.
 * @param from_arr_datatype: тип данных элементов массива `from_arr`.
 * @param to_arr: массив, куда будет записан результат сбора на процессе
 * `to_rank`.
 * @param to_arr_datatype: тип данных элементов массива `to_arr`.
 * @param to_arr_counts: количество значений, которое будет получено от каждого
 * процесса.
 * @param displacements: смещения в массиве `to_arr` для каждого
 * процесса.
 * @param arr_len: количество элементов в массиве `from_arr` и `to_arr`.
 */
#define parallel_GatherVariousArray(from_arr, from_arr_datatype, to_arr,    \
                                    to_arr_datatype, to_arr_counts,         \
                                    displacements, arr_len)                 \
  {                                                                         \
    if (PARALLEL_MACROS_NEED_PRINT)                                         \
      std::cout << "parallel_GatherVariousArray with args: from_arr: "      \
                << from_arr << "; from_arr_datatype: " << from_arr_datatype \
                << "; to_arr: " << to_arr                                   \
                << "; to_arr_datatype: " << to_arr_datatype                 \
                << "; to_arr_counts: " << to_arr_counts                     \
                << "; displacements: " << displacements                     \
                << "; arr_len: " << arr_len << "; to_rank: " << 0           \
                << "; comm: " << MPI_COMM_WORLD << std::endl;               \
    if (arr_len < 0)                                                        \
      parallel_Error(                                                       \
          "parallel_GatherVariousArray: arr_len should be non-negative.");  \
    parallel_CheckSuccess(MPI_Gatherv(from_arr, arr_len, from_arr_datatype, \
                                      to_arr, to_arr_counts, displacements, \
                                      to_arr_datatype, 0, MPI_COMM_WORLD)); \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;    \
  }

/**
 * @brief Собирает векторы от всех процессов в сети MPI в одном процессе с
 * различными размерами для каждого процесса.
 * @param from_vec: вектор, который будет отправлен от текущего процесса.
 * @param from_vec_datatype: тип данных элементов вектора `from_arr`.
 * @param to_vec: вектор, куда будет записан результат сбора на процессе
 * `to_rank`.
 * @param to_vec_datatype: тип данных элементов вектора `to_arr`.
 * @param to_vec_counts: количество значений, которое будет получено от каждого
 * процесса.
 * @param displacements: смещения в векторе `to_vec` для каждого
 * процесса.
 */
#define parallel_GatherVariousVector(from_vec, from_vec_datatype, to_vec,     \
                                     to_vec_datatype, to_vec_counts,          \
                                     displacements)                           \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_GatherVariousVector with args: from_vec: "       \
                << from_vec << "; from_vec_datatype: " << from_vec_datatype   \
                << "; to_vec: " << to_vec                                     \
                << "; to_vec_datatype: " << to_vec_datatype                   \
                << "; to_vec_counts: " << to_vec_counts                       \
                << "; displacements: " << displacements << "; to_rank: " << 0 \
                << "; comm: " << MPI_COMM_WORLD << std::endl;                 \
    if (from_vec.size() > INT_MAX || to_vec.size() > INT_MAX ||               \
        to_vec_counts.size() > INT_MAX || displacements.size() > INT_MAX)     \
      parallel_Error(                                                         \
          "parallel_GatherVariousVector: vector is too big (size > "          \
          "INT_MAX).");                                                       \
    parallel_GatherVariousArray(from_vec.data(), from_vec_datatype,           \
                                to_vec.data(), to_vec_datatype,               \
                                to_vec_counts.data(), displacements.data(),   \
                                Min(static_cast<int>(from_vec.size()),        \
                                    static_cast<int>(to_vec.size()),          \
                                    static_cast<int>(to_vec_counts.size()),   \
                                    static_cast<int>(displacements.size()))); \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

#endif
