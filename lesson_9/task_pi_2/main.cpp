#include <cmath>
#include <fstream>

#include "omp.h"
#include "utils.hpp"

double FourMinusSqrt(double x) { return sqrt(4.0 - x * x); }

int main() {
  double Pi = 0;

  int N = NumberFromFile("N.dat");

#pragma omp parallel for shared(N) reduction(+ : Pi)
  for (int i = 0; i < N; i++)
    Pi += (FourMinusSqrt(i * (2.0 / N)) + FourMinusSqrt((2.0 / N) * (i + 1))) /
          2 * (2.0 / N);

  std::cout << "Pi: " << Pi << std::endl;

  return 0;
}
