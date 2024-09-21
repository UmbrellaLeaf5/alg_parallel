// 1. группы + комм.
// 2. p-t-p
// 3. групповые опер.
// 4. топологии

int main(int argc, char *argv[]);
// Пример:
// ./a.out 156 bb
// argc = 3
// argv = [/*указатели на ]

// Объявление:
int MPI_Init(int *argc, char ***argv[]);

// Вызов:
int main(int argc, char *argv[]) { MPI_Init(&argc, &argv); }

// 2 функции, в которых можно не проверять MPI.SUCCESS:
// 1.
int MPI_Initialized(int *flag);
// 2.
int MPI_Finalize(void);

int MPI_Abort(MPI_Comm com, int code);

// Каркас первой программы:
int main(int argc, char *argv[]) {
  int st;
  st = MPI_Init(&argc, &argv);
  if (st != MPI_SUCCESS) {
    // ...
    MPI_Abort(MPI_COMM_WORLD, st);
    // ...
  }
  MPI_Finalize();
}

// отдельные компиляторы для использования MPI, не сильно парясь:
// c: mpicc
// c++: mpic++