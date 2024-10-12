# Lesson 4

## Task 1

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

## Task 2
