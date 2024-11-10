#pragma once

#include <mpi.h>

#include "utils.hpp"

/// @brief тег для сообщений MPI.
#define PARALLEL_STANDARD_TAG 35817

#ifndef PARALLEL_MACROS_NEED_PRINT
#define PARALLEL_MACROS_NEED_PRINT false
#endif

#ifndef PARALLEL_MACROS_DEFAULT_VALUES

#define parallel_Abort(state, comm) MPI_Abort(comm, state)

#define parallel_CheckSuccess(state, comm) \
  if (state != MPI_SUCCESS) parallel_Abort(state, comm)

#define parallel_Error(error_text, state, comm)                             \
  {                                                                         \
    std::cerr << "Error! Aborting because of: " << error_text << std::endl; \
    parallel_Abort(state, comm);                                            \
  }

#define parallel_Init(argc, argv, comm)                                  \
  {                                                                      \
    if (PARALLEL_MACROS_NEED_PRINT)                                      \
      std::cout << "parallel_Init with args: argc: " << argc             \
                << "; argv: " << argv << std::endl;                      \
    parallel_CheckSuccess(MPI_Init(&argc, &argv), comm);                 \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl; \
  }

#define parallel_Finalize(comm)                                          \
  {                                                                      \
    parallel_CheckSuccess(MPI_Finalize(), comm);                         \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl; \
  }

#define parallel_RanksAmount(ranks_amount, comm)                         \
  {                                                                      \
    if (PARALLEL_MACROS_NEED_PRINT)                                      \
      std::cout << "parallel_RanksAmount with args: comm:b" << comm      \
                << std::endl;                                            \
    parallel_CheckSuccess(MPI_Comm_size(comm, &ranks_amount), comm);     \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl; \
  }

#define parallel_CurrRank(curr_rank, comm)                                     \
  {                                                                            \
    if (PARALLEL_MACROS_NEED_PRINT)                                            \
      std::cout << "parallel_CurrRank with args: comm: " << comm << std::endl; \
    parallel_CheckSuccess(MPI_Comm_rank(comm, &curr_rank), comm);              \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;       \
  }

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

#define parallel_Abort() MPI_Abort(MPI_COMM_WORLD, 1)

#define parallel_CheckSuccess(state) \
  if (state != MPI_SUCCESS) parallel_Abort();

#define parallel_Error(error_text)                                          \
  {                                                                         \
    std::cerr << "Error! Aborting because of: " << error_text << std::endl; \
    parallel_Abort();                                                       \
  }

#define parallel_Init(argc, argv)                                        \
  {                                                                      \
    if (PARALLEL_MACROS_NEED_PRINT)                                      \
      std::cout << "parallel_Init with args: argc: " << argc             \
                << "; argv: " << argv << std::endl;                      \
    parallel_CheckSuccess(MPI_Init(&argc, &argv));                       \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl; \
  }

#define parallel_Finalize()                                              \
  {                                                                      \
    parallel_CheckSuccess(MPI_Finalize());                               \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl; \
  }

#define parallel_RanksAmount(ranks_amount)                                    \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_RanksAmount with args: comm: " << MPI_COMM_WORLD \
                << std::endl;                                                 \
    parallel_CheckSuccess(MPI_Comm_size(MPI_COMM_WORLD, &ranks_amount));      \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

#define parallel_CurrRank(curr_rank)                                       \
  {                                                                        \
    if (PARALLEL_MACROS_NEED_PRINT)                                        \
      std::cout << "parallel_CurrRank with args: comm: " << MPI_COMM_WORLD \
                << std::endl;                                              \
    parallel_CheckSuccess(MPI_Comm_rank(MPI_COMM_WORLD, &curr_rank));      \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;   \
  }

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

#define parallel_BroadcastValue(value, datatype)                              \
  {                                                                           \
    if (PARALLEL_MACROS_NEED_PRINT)                                           \
      std::cout << "parallel_BroadcastValue with args: value: " << value      \
                << "; datatype: " << datatype << "; from_rank: " << 0         \
                << "; comm: " << MPI_COMM_WORLD << std::endl;                 \
    parallel_CheckSuccess(MPI_Bcast(&value, 1, datatype, 0, MPI_COMM_WORLD)); \
    if (PARALLEL_MACROS_NEED_PRINT) std::cout << "SUCCESS" << std::endl;      \
  }

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
