#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "parallel.hpp"

/**
 * @brief Выводит все элементы вектора в поток
 * @tparam Type: тип, возможный к выводу в консоль
 * @param os: ссылка на поток, в который надо вывести (мод.)
 * @param vec: вектор элементов произвольного типа
 * @return std::ostream&: ссылка на поток, в который вывели
 */
template <typename Type>
inline std::ostream& operator<<(std::ostream& os,
                                const std::vector<Type>& vec) {
  os << "{";

  for (std::size_t i = 0; i < vec.size(); i++) {
    os << vec[i];
    if (i != vec.size() - 1) os << "; ";
  }

  return os << "}";
}

/**
 * @brief Записывает вектор в файл
 * @tparam Type: тип данных в векторе
 * @param vec: вектор
 * @param file_name: имя файла
 */
template <typename Type>
void VectorToFile(const std::vector<Type>& vec, const std::string& file_name) {
  std::ofstream out(file_name.c_str());

  if (!out.is_open())
    throw std::runtime_error("VectorToFile: file opening error.");

  out << vec;

  out.close();
}

int main(int argc, char* argv[]) {
  parallel::Init(argc, argv);

  int ranks_amount = parallel::RanksAmount();
  int curr_rank = parallel::CurrRank();

  const int N = 120;

  if (N % ranks_amount != 0) {
    throw std::runtime_error("main: N is not divisible by processes amount.");

    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  std::vector<int> curr_rank_vec(N / ranks_amount, curr_rank);

  std::vector<int> main_vec;
  if (curr_rank == 0) main_vec.resize(N);

  parallel::AbortTest(MPI_Gather(curr_rank_vec.data(), N / ranks_amount,
                                 MPI_INT, main_vec.data(), N / ranks_amount,
                                 MPI_INT, 0, MPI_COMM_WORLD));

  if (curr_rank == 0) VectorToFile(main_vec, "result.txt");

  parallel::Finalize();
}
