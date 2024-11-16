#include <cmath>
#include <fstream>

#include "omp.h"
#include "pi.hpp"
#include "utils.hpp"

int main() {
  double pi = 0;

  int N = NumberFromFile("N.dat");

#pragma omp parallel shared(N) reduction(+ : pi)
  {
    int threads_amount = omp_get_num_threads();
    int curr_thread = omp_get_thread_num();

    double part_of_pi = PartOfPi(N, curr_thread * (N / threads_amount),
                                 (curr_thread + 1) * (N / threads_amount));
    pi = part_of_pi;
  }

  std::cout << "Pi: " << pi << std::endl;

  return 0;
}
