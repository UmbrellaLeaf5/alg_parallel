# Lesson 9

```cpp
#include <stdio.h>

#include "omp.h"

int main() {
  static int x0 = 5;
#pragma omp threadprivate(x0)
#pragma omp parallel
  {
    x0 = omp_get_thread_num();

    printf("Parpart 1 %d %d\n", omp_get_thread_num(), x0);
  }

#pragma omp parallel
  {
    x0 += 1;

    printf("Parpart 2 %d %d\n", omp_get_thread_num(), x0);
  }

  printf("Seqpart 2 %d %d\n", omp_get_thread_num(), x0);

  return 0;
}
```

```cpp
#include <stdio.h>

#include "omp.h"

int main() {
  static int x0 = 5;
#pragma omp threadprivate(x0)
#pragma omp parallel
  {
    x0 = omp_get_thread_num();

    printf("Parpart 1 %d %d\n", omp_get_thread_num(), x0);
  }
  x0 = 2;

#pragma omp parallel copyin(x0)
  {
    x0 += 1;

    printf("Parpart 2 %d %d\n", omp_get_thread_num(), x0);
  }

  printf("Seqpart 2 %d %d\n", omp_get_thread_num(), x0);

  return 0;
}
```

```cpp
#include <stdio.h>

#include "omp.h"

int main() {
  int count = 0;

#pragma omp parallel reduction(+ : count)

  {
    count++;

    printf("Current value %d\n", count);
  }

  printf("It were %d threads\n", count);
}
```
Task Pi:

