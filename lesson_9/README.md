# Lesson 9

## Директива threadprivate

Директива `threadprivate` позволяет организовать приватные переменные, которые будут сохранять свои значения в разных параллельных областях для нитей исполнения с одинаковыми номерами.

```cpp
#pragma omp threadprivate (список)
```

Ограничения на использование: 
* применяется только к статическим переменным; 
* применяется до первого использования переменных из списка;
* в соответствующих параллельных областях должно работать одинаковое количество нитей исполнения;

### Пример:
```cpp
#include <stdio.h>

#include <omp.h>

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

## Директива copyin

Если необходимо переменную, объявленную как `threadprivate`, инициализировать значением размножаемой переменной из нити-мастера, то на входе в паралелльную область используется опция `copyin` прагмы `parallel`.

Общий вид опции:
```
copyin(имя[, имя[, ...]])
```

```cpp
#include <stdio.h>

#include <omp.h>

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

## Редукционные операции

Для реализации редукционных операций в параллельном блоке используется опция `reduction` директивы `parallel`.
```
reduction(оператор:имя[,[имя ...]])
```
* допустимые операторы: `+`, `-`, `*`, `&`, `|`, `^`, `&&`, `||`;
* имя – это идентификатор внешней скалярной переменной.

Для каждой переменной создаются локальные копии в нитях.

Локальные копии инициализируются по типу выполняемой операции

После выполнения всех операторов параллельной области над локальными копиями выполняется оператор.

Во внешнюю переменную заносится результат выполнения редукционного оператора над ее старым значением и результатом редукции в нитях.

## Пример подсчета количества нитей, работающих в параллельной части
```cpp
#include <stdio.h>

#include <omp.h>

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

## Task Pi:

Модифицируйте последовательную программу для определения значения числа `pi` так, чтобы расчет производился нитями исполнения. Количество нитей должно быть равно количеству ядер на узле.

## Распределение работы

Обычная работа нитей в директиве `parallel`
```cpp
#pragma omp parallel num_threads (3)
{
  th0   th1   th2
   |     |     |
   |     |     |
   |     |     |
   |     |     |

}
```

## Использование директивы single

Псевдо-примеры:
```cpp
#pragma omp parallel num_threads (3)
{    th0   th1   th2
#pragma omp single
{
     |          
     |   wait   wait
     |          
}
     |     |     |
     |     |     |
     |     |     |
}
```

```cpp
#pragma omp parallel num_threads (3)
{    th0   th1   th2
#pragma omp single
{
           |     
   wait    |    wait
           |     
}
     |     |     |
     |     |     |
     |     |     |
}
```

```cpp
#pragma omp parallel num_threads (3)
{    th0   th1   th2
#pragma omp single
{
                 |
   wait   wait   |
                 |
}
     |     |     |
     |     |     |
     |     |     |
}
```

## Использование директивы single nowait
```cpp
#pragma omp parallel num_threads (3)
{    th0   th1   th2
#pragma omp single nowait
{
                 |
                 |
                 |
}
     |     |     |
     |     |     |
     |     |     |
}
```

## Task Single:
Напишите программу с 2-мя директивами `single` в параллельной части.

В области директив `single` напечатайте номер директивы и номер работающей нити.

Между директивами `single` напечатайте номер работающей нити.

Объясните полученный результат

## Использование директивы master
(здесь отсутствует)

## Использование секций
(здесь отсутствует)

## Task Section:
Напишите программу с 6-ю директивами `section` в параллельной части.

В области директив `section` напечатайте номер директивы и номер работающей нити.

После окончания области директивы `sections` напечатайте номер работающей нити.

Объясните полученный результат

## Распараллеливание циклов

Для определения распараллеливаемого цикла в параллельной области применяется директива for.

Общий вид директивы:
```cpp
#pragma omp for [опция[[,] опция]...]
```

Распараллеливаются только циклы вида:
```cpp
for( i = инвариант; i {<, >, <=, >=} инвариант;
i {{{+,-} = инвариант}, ++, --})
```

Если директива стоит перед «гнездом циклов», то распараллеливается только самый внешний цикл.

Счетчик цикла автоматически становится приватным для нитей исполнения.

```cpp
#pragma omp parallel

{

/* ... */

#pragma omp for

for(i = 0; i < 100000; i++){
  a[i] = i;
}

/* ... */

}
```
## Task Pi 2:
Модифицируйте последовательную программу для определения значения числа π так, чтобы расчет производился нитями исполнения с использованием директивы for. 

Количество нитей должно быть равно количеству ядер на узле.

## Task SeqTeplo:

Модифицируйте последовательную программу для решения одномерного уравнения теплопроводности так, чтобы расчет производился с использованием директив OMP. Количество нитей должно быть равно количеству ядер на узле.