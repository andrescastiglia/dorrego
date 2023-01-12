![workflow](https://github.com/andrescastiglia/dorrego/actions/workflows/cmake.yml/badge.svg)


# Run

```
mpirun -n 2 ./build/dorrego
```


# Design

0 - coordinator
 - split responsabilities
    - wal
    - in channel
    - out channel
    - matching


1 - check balance if buy currency 0 otherwise order.currency
2 - ...