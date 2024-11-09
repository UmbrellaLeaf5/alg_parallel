# Lesson 8

```cpp
#pragma omp pragma_name [opt[[,] opt]...]
```

```shell
g++ -std=c++11 main.cpp -fopenmp
```

```cpp
#include <iostream>

// #include "omp.h"

int main() {
  // sequential part:

  std::cout << "Sequential Hello World!" << std::endl;

  // parallel part:
#pragma omp parallel
  { std::cout << "Parallel Hello World!" << std::endl; }

  return 0;
}
```

```cpp
#include <iostream>

#include "omp.h"

int main() {
  // sequential part:

  std::cout << "Sequential Hello World!" << std::endl;

  omp_set_num_threads(3);  // установка 3 нитей

  // parallel part:
#pragma omp parallel num_threads(5)  // установка 5 нитей
  { std::cout << "Parallel Hello World!" << std::endl; }

  return 0;
}
```

```cpp
// #include <iostream>
#include <stdio.h>

// #include "omp.h"

int x_0 = 2;

int main() {
  // sequential part:

  int x_1 = 2;

  // parallel part:
#pragma omp parallel shared(x_0, x_1)
  {
    int x_2 = 0;
    static int x_3 = 0;
    x_0++;
    x_1++;
    x_2++;
    x_3++;

    // std::cout << "x_0 = " << x_0 << " ; x_1 = " << x_1 << " ; x_2 = " << x_2
    //           << " ; x_3 = " << x_3 << std::endl;
    printf("x_0 = %d; x_1 = %d; x_2 = %d; x_3 = %d;\n", x_0, x_1, x_2, x_3);
  }

  // sequential part:

  // std::cout << "x_0 = " << x_0 << " ; x_1 = " << x_1 << std::endl;
  printf("x_0 = %d; x_1 = %d;\n", x_0, x_1);

  return 0;
}
```

```cpp
// #include <iostream>
#include <stdio.h>

// #include "omp.h"

int x_0 = 2;

int main() {
  // sequential part:

  int x_1 = 2;

  // parallel part:
#pragma omp parallel shared(x_0) private(x_1)
  {
    int x_2 = 0;
    static int x_3 = 0;
    x_0++;
    x_1++;
    x_2++;
    x_3++;

    // std::cout << "x_0 = " << x_0 << " ; x_1 = " << x_1 << " ; x_2 = " << x_2
    //           << " ; x_3 = " << x_3 << std::endl;
    printf("x_0 = %d; x_1 = %d; x_2 = %d; x_3 = %d;\n", x_0, x_1, x_2, x_3);
  }

  // sequential part:

  // std::cout << "x_0 = " << x_0 << " ; x_1 = " << x_1 << std::endl;
  printf("x_0 = %d; x_1 = %d;\n", x_0, x_1);

  return 0;
}
```

```cpp
int main() {
  // sequential part:
  { int a = 2, b = 1, c[10], d[2] = {1, 1}; }
  // parallel part:
#pragma omp parallel shared(b, c) private(a, d)
  {}
}
```

Task Hello, OMP!