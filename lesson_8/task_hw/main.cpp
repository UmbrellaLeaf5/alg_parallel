// #include <iostream>
#include <omp.h>
#include <stdio.h>


int main() {
  // sequential part:

  int procs_amount = omp_get_num_procs(), threads_amount, curr_thread;

  printf("Processes amount: %d\n\n", procs_amount);

  // parallel part:
#pragma omp parallel num_threads(6) shared(threads_amount, curr_thread)
  {
    threads_amount = omp_get_num_threads();
    curr_thread = omp_get_thread_num();
    printf("Hello, OMP!\n Threads amount: %d\n Current thread: %d\n\n",
           threads_amount, curr_thread);
  }

  // sequential part:

  threads_amount = omp_get_num_threads();
  curr_thread = omp_get_thread_num();
  printf("Threads amount: %d\nCurrent thread: %d\n", threads_amount,
         curr_thread);

  return 0;
}
