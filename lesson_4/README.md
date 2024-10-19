# Lesson 4

## Task 1: Send ring

```C++
int i = 0;
```

```
size: N

proc 0 -> proc 1: i = i
proc 1 -> proc 2: i = i + rank
proc 2 -> proc 3: i = i + rank
...
proc N-1 -> proc 0: i = i + rank
```

## Task 2: Send Pi sum
```
N.dat

rank 0 -> N - в цикле рассылка
rank != <- N
  расчет суммы
rank != 0 -> S_k

только в цикле прием S_k*sum(S_k)
```
