#include <cmath>
#include <fstream>

#include "omp.h"
#include "pi.hpp"
#include "utils.hpp"

int main() {
  double pi = 0;

  int N = NumberFromFile("N.dat");

#pragma omp parallel for shared(N) reduction(+ : pi)
  for (int i = 0; i < N; i++)
    pi += (SqrtFourMinusSqr(i * (2.0 / N)) +
           SqrtFourMinusSqr((2.0 / N) * (i + 1))) /
          2 * (2.0 / N);

  std::cout << "Pi: " << pi << std::endl;

  return 0;
}
