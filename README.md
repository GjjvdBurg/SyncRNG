SyncRNG
=======
A synchronized Tausworthe RNG usable in R and Python.


Why?
----

This program was created because it was desired to have the same random 
numbers in both R and Python programs. Although both languages implement a 
Mersenne-Twister RNG, the implementations are so different that it is not 
possible to get the same random numbers with the same seed.

SyncRNG is a Tausworthe RNG implemented in `syncrng.c`, and linked to both R 
and Python. Since both use the same underlying C code, the random numbers will 
be the same in both languages, provided the same seed is used.

How
---

In Python, the interface `SyncRNG.py` can be used as an importable module. In 
R, it suffices to simply source the `SyncRNG.R` file. Before use, make sure to 
build both shared libraries using:

    make

Then, in a Python script located in the same directory as `syncrng.so` and 
`SyncRNG.py`, you can do:

```python
from SyncRNG import SyncRNG

s = SyncRNG(seed=123456)
for i in range(10):
  print(s.randi())
```

Similarly, in an R script located in the same directory as `RSyncRNG.so` and 
`SyncRNG.R`, you can do:

```R
source('./SyncRNG.R')

s = SyncRNG(seed=123456)
for (i in 1:10) {
	cat(s$randi(), '\n')
}
```

You'll notice that the random numbers are indeed the same.

Notes
-----

The random numbers are uniformly distributed on `[0, 2^32 - 1]`.


TODO
----

It may be easier to provide system-wide installation through an R package and 
a Python module.
