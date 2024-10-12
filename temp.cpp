#include <cmath>
#include <iostream>
#include <vector>

double FourMinusSqrt(double x) { return sqrt(4.0 - x * x); }

double Area(double x, double seg) {
  return (FourMinusSqrt(x) + FourMinusSqrt(x + seg)) * seg / 2;
}

double PartOfPi(int N, int start, int end) {
  double part_of_pi = 0;
  double seg = 2.0 / N;
  double curr_pos = double(start) / N * 2;

  for (short i = start; i < end; i++) {
    part_of_pi += Area(curr_pos, seg);
    curr_pos += seg;
  }

  return part_of_pi;
}

double Pi(int N) { return PartOfPi(N, 0, N); }

int main() {
  auto n = 20000;
  auto size = 5;

  std::cout << Pi(n) << std::endl;

  double sum = 0;
  double sum2 = 0;

  for (int i = 0; i < n; i += n / size) {
    std::cout << i << " " << i + n / size << std::endl;
    sum += PartOfPi(n, i, i + n / size);
  }

  for (int i = 0; i < size; i++) {
    std::cout << i * (n / size) << " " << (i + 1) * (n / size) << std::endl;
    sum2 += PartOfPi(n, i * (n / size), (i + 1) * (n / size));
  }

  std::cout << sum << std::endl;
  std::cout << sum2 << std::endl;

  return 0;
}