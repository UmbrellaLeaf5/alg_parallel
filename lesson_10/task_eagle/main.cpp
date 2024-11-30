#include <omp.h>
#include <stdio.h>
#include <unistd.h>


int main() {
  // sequential part:

  // printf("procs_amount: %d\n", omp_get_num_procs());

  short meat_amount = 5;
  constexpr short eagle_amount = 3;
  short mama_eagle_flights = 3;

  bool is_meat_needed = false;

  // parallel part:
#pragma omp parallel num_threads(eagle_amount + 1)
  {
    int curr_eagle = omp_get_thread_num();
    bool is_mama_eagle = (curr_eagle == 0);

    while (mama_eagle_flights != 0) {
      if (is_mama_eagle) {
        while (!is_meat_needed);
        is_meat_needed = false;

        mama_eagle_flights--;
        printf("Mama eagle flies searching meat...\n");

        sleep(3);

        printf("Mama eagle returns with meat!\n");
        meat_amount = 5;

      } else {
#pragma omp critical
        {
          if (meat_amount == 0) {
            printf("Eagle number %d can't find meat!\n", curr_eagle);
            is_meat_needed = true;

            while (meat_amount == 0);
          }

          meat_amount--;
          printf("Eagle number %d eats 1 meat.\n", curr_eagle);
        }

        sleep(1);
      }
    }
  }

  // sequential part:

  return 0;
}

// FIXME: Орлята не доедают последние принесенные 4 курса