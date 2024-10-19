# Lesson 2

1. группы + комм.
2. p-t-p
3. групповые опер.
4. топологии

```C++
int main(int argc, char *argv[]);
// Пример:
// ./a.out 156 bb
// argc = 3
// argv = [/*указатели на ]
```

Объявление:
```C++
int MPI_Init(int *argc, char ***argv[]);
```

Вызов:
```C++
int main(int argc, char *argv[]) { MPI_Init(&argc, &argv); }
```

2 функции, в которых можно не проверять MPI.SUCCESS:
```C++
// 1.
int MPI_Initialized(int *flag);
// 2.
int MPI_Finalize(void);
```
Если не `MPI_SUCCESS`:
```C++
int MPI_Abort(MPI_Comm com, int code);
```

Каркас первой программы:
```C++
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
```

Отдельные компиляторы для использования MPI, не сильно парясь:
`c: mpicc`
`c++: mpic++`