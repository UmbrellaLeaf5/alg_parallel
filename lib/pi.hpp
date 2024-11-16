#pragma once

#include <cmath>
#include <iostream>

/**
 * @brief Вычисляет значение sqrt(4.0 - x^2)
 * @param x: входной аргумент
 * @return double: результат вычисления
 */
inline double SqrtFourMinusSqr(double x) { return sqrt(4.0 - x * x); }

/**
 * @brief Считает площадь кусочка полуокружности
 * @param x: начальная координата
 * @param seg: длина счетного отрезка
 * @return double: значение площади
 */
inline double Area(double x, double seg) {
  return (SqrtFourMinusSqr(x) + SqrtFourMinusSqr(x + seg)) * seg / 2.0;
}

/**
 * @brief Вычисляет часть значения числа Пи
 * @param N: количество частей, на которые делится полуокружность
 * @param start: начальный индекс части
 * @param end: конечный индекс части (не включая)
 * @return double: часть значения числа Пи
 */
inline double PartOfPi(int N, int start, int end) {
  double part_of_pi = 0;
  double seg = 2.0 / N;
  double curr_pos = double(start) / N * 2.0;

  for (int i = start; i < end; i++) {
    if (std::isfinite(Area(curr_pos, seg))) part_of_pi += Area(curr_pos, seg);
    curr_pos += seg;
  }

  return part_of_pi;
}

// inline double Pi(int N) { return PartOfPi(N, 0, N); }

/**
 * @brief Вычисляет значение числа Пи
 * @param N: количество частей, на которые делится полуокружность
 * @return double: значение числа Пи
 */
inline double Pi(int N) {
  double pi = 0;
  double seg = 2.0 / N;
  double curr_pos = 0;

  for (short i = 0; i < N; i++) {
    if (std::isfinite(Area(curr_pos, seg))) pi += Area(curr_pos, seg);
    curr_pos += seg;
  }

  return pi;
}
