#include <stdio.h>

#include "omp.h"

int main() {
  int curr_thread;

#pragma omp parallel private(curr_thread)
  {
#pragma omp sections
    {
#pragma omp section
      {
        curr_thread = omp_get_thread_num();
        printf("Section 1 current thread: %d\n", curr_thread);
      }
#pragma omp section
      {
        curr_thread = omp_get_thread_num();
        printf("Section 2 current thread: %d\n", curr_thread);
      }
#pragma omp section
      {
        curr_thread = omp_get_thread_num();
        printf("Section 3 current thread: %d\n", curr_thread);
      }
#pragma omp section
      {
        curr_thread = omp_get_thread_num();
        printf("Section 4 current thread: %d\n", curr_thread);
      }
#pragma omp section
      {
        curr_thread = omp_get_thread_num();
        printf("Section 5 current thread: %d\n", curr_thread);
      }
#pragma omp section
      {
        curr_thread = omp_get_thread_num();
        printf("Section 6 current thread: %d\n", curr_thread);
      }
    }

    curr_thread = omp_get_thread_num();
    printf("Just parallel: %d\n", curr_thread);
  }
}