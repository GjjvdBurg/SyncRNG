=======
SyncRNG
=======
A synchronized Tausworthe RNG usable in R and Python.

Why?
====

This program was created because it was desired to have the same random 
numbers in both R and Python programs. Although both languages implement a 
Mersenne-Twister RNG, the implementations are so different that it is not 
possible to get the same random numbers with the same seed.

SyncRNG is a Tausworthe RNG implemented in `syncrng.c`, and linked to both R 
and Python. Since both use the same underlying C code, the random numbers will 
be the same in both languages, provided the same seed is used.

How
===

First install the packages as stated under Installation. Then, in Python you 
can do::

    from SyncRNG import SyncRNG

    s = SyncRNG(seed=123456)
    for i in range(10):
      print(s.randi())

Similarly, after installing the R library you can do in R::

    library(SyncRNG)

    s <- SyncRNG(seed=123456)
    for (i in 1:10) {
       cat(s$randi(), '\n')
    }

You'll notice that the random numbers are indeed the same.

Installation
============

Installing the R package can be done through CRAN::

    install.packages('SyncRNG')

The Python package can be installed using pip::

    pip install syncrng


Usage
=====

In both R and Python the following methods are available for the `SyncRNG` 
class:

1. `randi()`: generate a random integer on the interval [0, 2^32).
2. `rand()`: generate a random floating point number on the interval [0.0, 
   1.0)
3. `randbelow(n)`: generate a random integer below a given integer `n`.
4. `shuffle(x)`: generate a permutation of a given list of numbers `x`.

Notes
=====

The random numbers are uniformly distributed on `[0, 2^32 - 1]`.


