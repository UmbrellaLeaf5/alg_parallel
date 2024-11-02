#pragma once

#include <climits>
#include <fstream>
#include <iomanip>
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
 * @brief Записывает вектор в файл
 * @tparam Type: тип данных в векторе
 * @param vec: вектор
 * @param precision: точность
 * @param file_name: имя файла
 */
inline void VectorToFileWithPrecision(
    const std::vector<double>& vec, int precision = 6,
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

/**
 * @brief Минимальное значение из двух аргументов
 * @tparam T: тип данных аргументов
 * @param a: первый аргумент
 * @param b: второй аргумент
 * @return T: минимальное значение из `a` и `b`
 */
template <typename T>
inline T Min(T a, T b) {
  return a < b ? a : b;
}

/**
 * @brief Минимальное значение из набора аргументов
 * @tparam T: тип данных аргументов
 * @tparam Args: типы данных остальных аргументов
 * @param a: первый аргумент
 * @param args: остальные аргументы
 * @return T: минимальное значение из всех аргументов.
 */
template <typename T, typename... Args>
inline T Min(T a, Args... args) {
  return Min(a, Min(args...));
}