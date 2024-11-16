#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "omp.h"

void Error(const char* format, ...) {
  // вывод сообщений об ошибках в stderr
  va_list args;
  va_start(args, format);

  vfprintf(stderr, format, args);

  va_end(args);
  exit(-1);
}

double Max(double a, double b) { return (a > b) ? a : b; }

int main(int argc, char* argv[]) {
  if (argc != 2) Error("Usage: .exe file n points.\n");

  double *U, *U_new, maxdelta, eps = 1.e-6, h, tau, *proc_max_array;
  int N = atoi(argv[1]), count = 0, i;

  if (N <= 0) {
    N = 1000;
    printf("Set N to 1000.\n");
  } else
    printf("Set N to %d.\n", N);

  if ((U = (double*)malloc((N + 1) * sizeof(double))) == NULL)
    Error("Can't allocate memory for U!\n");

  if ((U_new = (double*)malloc((N + 1) * sizeof(double))) == NULL) {
    free(U);

    Error("Can't allocate memory for U_new!\n");
  }

  // begin & bound values

  for (i = 1; i < N; i++) U[i] = 0.0;

  U_new[0] = U[0] = 1.0;
  U_new[N] = U[N] = 0.0;

  h = 1.0 / N;
  tau = 0.5 * (h * h);

  if ((proc_max_array =
           (double*)malloc((omp_get_num_procs()) * sizeof(double))) == NULL) {
    free(U);
    free(U_new);

    Error("Can't allocate memory for U_new!\n");
  }

  for (;;) {
    maxdelta = 0;

#pragma omp parallel for
    for (i = 1; i < N; i++) {
      U_new[i] = U[i] + (tau / (h * h)) * (U[i - 1] - 2 * U[i] + U[i + 1]);

      proc_max_array[omp_get_thread_num()] =
          Max(proc_max_array[omp_get_thread_num()], fabs(U_new[i] - U[i]));
    }

#pragma omp single
    {
      maxdelta = proc_max_array[0];

      for (i = 0; i < omp_get_num_procs(); i++) {
        maxdelta = Max(maxdelta, proc_max_array[i]);
        proc_max_array[i] = 0;
      }
    }

    if (maxdelta < eps) break;
    count++;

#pragma omp parallel for
    for (i = 1; i < N; i++) U[i] = U_new[i];
  }

  FILE* file;

  printf("%d steps\n", count);
  if ((file = fopen("seqres", "w+")) == NULL) {
    free(U);
    free(U_new);
    free(proc_max_array);

    Error("Can't open file\n");
  }

  for (i = 0; i < N + 1; i++) fprintf(file, "%f\n", U_new[i]);

  fclose(file);
  free(U);
  free(U_new);
  free(proc_max_array);

  return 0;
}