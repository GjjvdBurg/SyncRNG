# SyncRNG

[![build](https://github.com/GjjvdBurg/SyncRNG/workflows/build/badge.svg)](https://github.com/GjjvdBurg/SyncRNG/actions)
[![CRAN version](https://www.r-pkg.org/badges/version/SyncRNG)](https://cran.r-project.org/web/packages/SyncRNG/index.html)
[![CRAN package downloads](https://cranlogs.r-pkg.org/badges/grand-total/SyncRNG)](https://cran.r-project.org/web/packages/SyncRNG/index.html)
[![PyPI version](https://badge.fury.io/py/SyncRNG.svg)](https://pypi.org/project/SyncRNG)
[![Python package downloads](https://pepy.tech/badge/SyncRNG)](https://pepy.tech/project/SyncRNG)

*Generate the same random numbers in R and Python.*

**Useful Links:**

- [SyncRNG on GitHub](https://github.com/GjjvdBurg/SyncRNG)
- [SyncRNG on PyPI](https://pypi.org/project/SyncRNG/)
- [SyncRNG on CRAN](https://cran.r-project.org/web/packages/SyncRNG/index.html)
- [Blog post on SyncRNG](https://gertjanvandenburg.com/blog/syncrng/)

*Contents:* <a href="#introduction"><b>Introduction</b></a> |
<a href="#installation"><b>Installation</b></a> |
<a href="#usage"><b>Usage</b></a> |
<a href="#functionality">Functionality</a> |
<a href="#r-user-defined-rng">R: User defined RNG</a> |
<a href="#examples"><b>Examples</b></a> |
<a href="#sampling-without-replacement">Sampling without replacement</a> |
<a href="#sampling-with-replacement">Sampling with replacement</a> |
<a href="#generating-normally-distributed-values">Generating Normally Distributed Values</a> |
<a href="#creating-the-same-traintest-splits">Creating the same train/test splits</a> |
<a href="#notes"><b>Notes</b></a>

## Introduction

I created this package because I needed to have the same random numbers in 
both R and Python programs. Although both languages implement a 
Mersenne-Twister random number generator (RNG), the implementations are so 
different that it is not possible to get the same random numbers, even with 
the same seed.

SyncRNG is a "Tausworthe" RNG implemented in C and linked to both R and 
Python. Since both use the same underlying C code, the random numbers will be 
the same in both languages when the same seed is used. A [Tausworthe 
generator](https://en.wikipedia.org/wiki/List_of_random_number_generators#Pseudorandom_number_generators_(PRNGs)) 
is based on a linear feedback shift register and relatively easy to implement.

You can read more about my motivations for creating this 
[here](https://gertjanvandenburg.com/blog/syncrng/).

If you use SyncRNG in your work, please consider citing it. Here is a BibTeX 
entry you can use:

```bibtex
@misc{vandenburg2015syncrng,
  author={{Van den Burg}, G. J. J.},
  title={{SyncRNG}: Synchronised Random Numbers in {R} and {Python}},
  url={https://github.com/GjjvdBurg/SyncRNG},
  year={2015},
  note={Version 1.3}
}
```

## Installation

Installing the R package can be done through CRAN:

```
> install.packages('SyncRNG')
```

The Python package can be installed using pip:

```
$ pip install syncrng
```

## Usage

After installing the package, you can use the basic ``SyncRNG`` random number 
generator. In Python you can do:


```python
>>> from SyncRNG import SyncRNG
>>> s = SyncRNG(seed=123456)
>>> for i in range(10):
>>>     print(s.randi())
```

And in R you can use:

```r
> library(SyncRNG)
> s <- SyncRNG(seed=123456)
> for (i in 1:10) {
>    cat(s$randi(), '\n')
> }
```

You'll notice that the random numbers are indeed the same.

### Functionality

In both R and Python the following methods are available for the ``SyncRNG`` 
class:

1. ``randi()``: generate a random integer on the interval [0, 2^32).
2. ``rand()``: generate a random floating point number on the interval [0.0, 
   1.0)
3. ``randbelow(n)``: generate a random integer below a given integer ``n``.
4. ``shuffle(x)``: generate a permutation of a given list of numbers ``x``.

Functionality is deliberately kept minimal to make maintaining this library 
easier. It is straightforward to build more advanced applications on the 
existing methods, as the examples below show.

### R: User defined RNG

R allows the user to define a custom random number generator, which is then 
used for the common ``runif`` function in R. This has also been implemented in 
SyncRNG as of version 1.3.0. To enable this, run:

```r
> library(SyncRNG)
> set.seed(123456, 'user', 'user')
> runif(10)
```

These numbers are between [0, 1) and multiplying by ``2**32 - 1`` gives the 
same results as above. Note that while this works for low-level random number 
generation using ``runif``, it is not guaranteed that higher-level functions 
that build on this (such as ``rnorm`` and ``sample``) translate easily to 
similar functions in Python. This has likely to do with R's internal 
implementation for these functions. Using random number primitives from 
SyncRNG directly is therefore generally more reliable. See the examples below 
for sampling and generating normally distributed values with SyncRNG.

## Examples

This section contains several examples of functionality that can easily be 
built on top of the primitives that SyncRNG provides.

### Sampling without replacement

Sampling without replacement can be done by leveraging the builtin ``shuffle`` 
method of SyncRNG:

R:
```r
> library(SyncRNG)
> v <- 1:10
> s <- SyncRNG(seed=42)
> # Sample 5 values without replacement
> s$shuffle(v)[1:5]
[1] 6 9 2 4 5
```

Python:
```python
>>> from SyncRNG import SyncRNG
>>> v = list(range(1, 11))
>>> s = SyncRNG(seed=42)
>>> # Sample 5 values without replacement
>>> s.shuffle(v)[:5]
[6, 9, 2, 4, 5]
```

### Sampling with replacement

Sampling with replacement simply means generating random array indices. Note 
that these values are not (necessarily) the same as what is returned from R's 
``sample`` function, even if we specify SyncRNG as the user-defined RNG (see 
above).

R:
```r
> library(SyncRNG)
> v <- 1:10
> s <- SyncRNG(seed=42)
> u <- NULL
> # Sample 15 values with replacement
> for (k in 1:15) {
+ idx <- s$randi() %% length(v) + 1
+ u <- c(u, v[idx])
+ }
> u
[1] 10  1  1  9  3 10 10 10  9  4  1  9  6  3  6
```

Python:
```python
>>> from SyncRNG import SyncRNG
>>> v = list(range(1, 11))
>>> s = SyncRNG(seed=42)
>>> u = []
>>> for k in range(15):
...     idx = s.randi() % len(v)
...     u.append(v[idx])
...
>>> u
[10, 1, 1, 9, 3, 10, 10, 10, 9, 4, 1, 9, 6, 3, 6]
```

### Generating Normally Distributed Values

It is also straightforward to implement a [Box-Muller 
transform](https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform) to 
generate normally distributed samples.

R:
```r
library(SyncRNG)

# Generate n numbers from N(mu, sigma^2)
syncrng.box.muller <- function(mu, sigma, n, seed=0, rng=NULL)
{
    if (is.null(rng)) {
        rng <- SyncRNG(seed=seed)
    }

    two.pi <- 2 * pi
    ngen <- ceiling(n / 2)
    out <- replicate(2 * ngen, 0.0)

    for (i in 1:ngen) {
        u1 <- 0.0
        u2 <- 0.0

        while (u1 == 0) { u1 <- rng$rand(); }
        while (u2 == 0) { u2 <- rng$rand(); }

        mag <- sigma * sqrt(-2.0 * log(u1))
        z0 <- mag * cos(two.pi * u2) + mu
        z1 <- mag * sin(two.pi * u2) + mu

        out[2*i - 1] = z0;
        out[2*i] = z1;
    }
    return(out[1:n]);
}

> syncrng.box.muller(1.0, 3.0, 11, seed=123)
 [1]  9.6062905  1.4132851  1.0223211  1.7554504 13.5366881  1.0793818
 [7]  2.5734537  1.1689116  0.5588834 -6.1701509  3.2221119
```

Python:
```python
import math
from SyncRNG import SyncRNG

def syncrng_box_muller(mu, sigma, n, seed=0, rng=None):
    """Generate n numbers from N(mu, sigma^2)"""
    rng = SyncRNG(seed=seed) if rng is None else rng

    two_pi = 2 * math.pi
    ngen = math.ceil(n / 2)
    out = [0.0] * 2 * ngen

    for i in range(ngen):
        u1 = 0.0
        u2 = 0.0

        while u1 == 0:
            u1 = rng.rand()
        while u2 == 0:
            u2 = rng.rand()

        mag = sigma * math.sqrt(-2.0 * math.log(u1))
        z0 = mag * math.cos(two_pi * u2) + mu
        z1 = mag * math.sin(two_pi * u2) + mu

        out[2*i] = z0
        out[2*i + 1] = z1

    return out[:n]

>>> syncrng_box_muller(1.0, 3.0, 11, seed=123)
[9.60629048280169, 1.4132850614143178, 1.0223211130311138, 1.7554504380249232, 
13.536688052073458, 1.0793818230927306, 2.5734537321359925, 
1.1689116061110083, 0.5588834007200677, -6.1701508943037195, 
3.2221118937024342]
```


### Creating the same train/test splits

A common use case for this package is to create the same train and test splits 
in R and Python. Below are some code examples that illustrate how to do this. 
Both assume you have a matrix ``X`` with `100` rows.

R:
```r

# This function creates a list with train and test indices for each fold
k.fold <- function(n, K, shuffle=TRUE, seed=0)
{
	idxs <- c(1:n)
	if (shuffle) {
		rng <- SyncRNG(seed=seed)
		idxs <- rng$shuffle(idxs)
	}

	# Determine fold sizes
        fsizes <- c(1:K)*0 + floor(n / K)
        mod <- n %% K
        if (mod > 0)
		fsizes[1:mod] <- fsizes[1:mod] + 1

        out <- list(n=n, num.folds=K)
	current <- 1
        for (f in 1:K) {
		fs <- fsizes[f]
		startidx <- current
		stopidx <- current + fs - 1
		test.idx <- idxs[startidx:stopidx]
		train.idx <- idxs[!(idxs %in% test.idx)]
		out$testidxs[[f]] <- test.idx
		out$trainidxs[[f]] <- train.idx
		current <- stopidx
	}
	return(out)
}

# Which you can use as follows
folds <- k.fold(nrow(X), K=10, shuffle=T, seed=123)
for (f in 1:folds$num.folds) {
        X.train <- X[folds$trainidx[[f]], ]
        X.test <- X[folds$testidx[[f]], ]

        # continue using X.train and X.test here
}
```

Python:
```python
def k_fold(n, K, shuffle=True, seed=0):
    """Generator for train and test indices"""
    idxs = list(range(n))
    if shuffle:
        rng = SyncRNG(seed=seed)
        idxs = rng.shuffle(idxs)

    fsizes = [n // K]*K
    mod = n % K
    if mod > 0:
        fsizes[:mod] = [x+1 for x in fsizes[:mod]]

    current = 0
    for fs in fsizes:
        startidx = current
        stopidx = current + fs
        test_idx = idxs[startidx:stopidx]
        train_idx = [x for x in idxs if not x in test_idx]
        yield train_idx, test_idx
        current = stopidx

# Which you can use as follows
kf = k_fold(X.shape[0], K=3, shuffle=True, seed=123)
for trainidx, testidx in kf:
    X_train = X[trainidx, :]
    X_test = X[testidx, :]

    # continue using X_train and X_test here
```

## Notes

The random numbers are uniformly distributed on ``[0, 2^32 - 1]``. No 
attention has been paid to thread-safety and you shouldn't use this random 
number generator for cryptographic applications.

If you have questions, comments, or suggestions about SyncRNG or you encounter 
a problem, please open an issue [on 
GitHub](https://github.com/GjjvdBurg/SyncRNG/). Please don't hesitate to 
contact me, you're helping to make this project better for everyone! If you 
prefer not to use Github you can email me at ``gertjanvandenburg at gmail dot 
com``.
