#include <omp.h>
#include <stdio.h>


int main() {
  int curr_thread;

#pragma omp parallel private(curr_thread)
  {
#pragma omp single
    {
      curr_thread = omp_get_thread_num();
      printf("First single. Current thread: %d\n", curr_thread);
    }

    curr_thread = omp_get_thread_num();
    printf("Just parallel. Current thread: %d\n", curr_thread);

#pragma omp single
    {
      curr_thread = omp_get_thread_num();
      printf("Second single. Current thread: %d\n", curr_thread);
    }
  }
}
