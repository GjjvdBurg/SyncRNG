# SyncRNG

[![build](https://github.com/GjjvdBurg/SyncRNG/workflows/build/badge.svg)](https://github.com/GjjvdBurg/SyncRNG/actions)
[![CRAN version](https://www.r-pkg.org/badges/version/SyncRNG)](https://cran.r-project.org/web/packages/SyncRNG/index.html)
[![CRAN package downloads](https://cranlogs.r-pkg.org/badges/grand-total/SyncRNG)](https://cran.r-project.org/web/packages/SyncRNG/index.html)
[![PyPI version](https://badge.fury.io/py/SyncRNG.svg)](https://pypi.org/project/SyncRNG)
[![Python package downloads](https://pepy.tech/badge/SyncRNG)](https://pepy.tech/project/SyncRNG)

Generate the same random numbers in R and Python.

## Why?

This program was created because I wanted to have the same random numbers in 
both R and Python programs. Although both languages implement a 
Mersenne-Twister random number generator (RNG), the implementations are so 
different that it is not possible to get the same random numbers, even with 
the same seed.

SyncRNG is a "Tausworthe" RNG implemented in C and linked to both R and 
Python. Since both use the same underlying C code, the random numbers will be 
the same in both languages when the same seed is used.

You can read more about my motivations for creating this 
[here](https://gertjanvandenburg.com/blog/syncrng/).

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

### R: User defined RNG

R allows the user to define a custom random number generator, which is then 
used for the common ``runif`` and ``rnorm`` functions in R. This has also been 
implemented in SyncRNG as of version 1.3.0. To enable this, run:

```r
> library(SyncRNG)
> set.seed(123456, 'user', 'user')
> runif(10)
```

These numbers are between [0, 1) and multiplying by ``2**32 - 1`` gives the 
same results as above.

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
existing methods, as the following example shows.

### Creating the same train/test splits

A common use case for this package is to create the same train and test splits 
in R and Python. Below are some code examples that illustrate how to do this. 
Both assume you have a matrix ``X`` with `100` rows.

In R:

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

And in Python:

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

## Questions and Issues

If you have questions, comments, or suggestions about SyncRNG or you encounter 
a problem, please open an issue [on 
GitHub](https://github.com/GjjvdBurg/SyncRNG/). Please don't hesitate to 
contact me, you're helping to make this project better for everyone! If you 
prefer not to use Github you can email me at ``gertjanvandenburg at gmail dot 
com``.
