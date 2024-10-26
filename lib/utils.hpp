#pragma once

#include <iostream>
#include <vector>

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
  // os << "{";

  for (std::size_t i = 0; i < vec.size(); i++) {
    os << vec[i];
    if (i != vec.size() - 1) os << " ";
    // << ";"
  }

  // os << "}";
  return os;
}

/**
 * @brief Записывает вектор в файл
 * @tparam Type: тип данных в векторе
 * @param vec: вектор
 * @param file_name: имя файла
 */
template <typename Type>
inline void VectorToFile(const std::vector<Type>& vec,
                         const std::string& file_name) {
  std::ofstream out(file_name.c_str());

  if (!out.is_open())
    throw std::runtime_error("VectorToFile: file opening error.");

  out << vec;

  out.close();
}