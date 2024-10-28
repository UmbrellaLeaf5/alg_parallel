#pragma once

#include <iostream>
#include <sstream>
#include <string>
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
                         const std::string& file_name = "results.txt") {
  std::ofstream out(file_name.c_str());

  if (!out.is_open()) {
    std::cerr << "VectorToFile: file opening error." << std::endl;
    return;
  }

  out << vec;

  out.close();
}

/**
 * @brief Считывает число из файла (из первой строки)
 * @param file_name: название файла (по умолчанию "N.dat")
 * @return int: число
 */
inline int NumberFromFile(const std::string& filename) {
  std::ifstream in(filename.c_str());
  std::string line;

  if (!in.is_open()) {
    std::cerr << "NumberFromFile: file opening error." << std::endl;
    return -1;
  }

  int number;
  in >> number;

  in.close();

  return number;
}

/**
 * @brief Конвертирует тип, для которого определна операция ввода в std::string
 * @tparam T: тип
 * @param value: значение
 * @return std::string: выходная строка
 */
template <typename T>
std::string ToString(T value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}