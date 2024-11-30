#include <omp.h>
#include <stdio.h>


// MARK: wrong realization:

#if (0)

int main() {
  // sequential part:

  int count = 0;

  // parallel part:
#pragma omp parallel num_threads(2)
  for (int j = 0; j < 1000000; j++) count++;

  // sequential part:

  printf("count = %d\n", count);
}

// MARK: right realization:

int main() {
  // sequential part:
  printf("procs_amount: %d\n", omp_get_num_procs());

  int count;
  int ready[2] = {0, 0};
  int turn;

  // parallel part:
#pragma omp parallel num_threads(2) shared(count, ready, turn)
  {
    int i = omp_get_thread_num();
    for (int j = 0; j < 1000000; j++) {
      ready[i] = 1;
      turn = 1 - i;

#pragma omp flush(turn, ready)
      while (ready[1 - i] && turn == 1 - i);
      count++;

      ready[i] = 0;
    }
  }

  // sequential part:

  printf("count = %d\n", count);
}

#endif

int main() {
  // sequential part:

  printf("procs_amount: %d\n", omp_get_num_procs());

  int count = 0;

  // parallel part:
#pragma omp parallel num_threads(2)
  {
    for (int j = 0; j < 1000000; j++)
#pragma omp critical
      count++;
  }

  // sequential part:

  printf("count = %d\n", count);
}