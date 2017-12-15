library(SyncRNG)
context("SyncRNG unit tests")

test_that("test_randi", {
  s <- SyncRNG(seed=123456)
  expect_equal(s$randi(), 959852049)
  expect_equal(s$randi(), 2314333085)
  expect_equal(s$randi(), 2255782734)
  expect_equal(s$randi(), 2921461239)
  expect_equal(s$randi(), 1024197102)
})

test_that("test_rand", {
  s <- SyncRNG(seed=123456)
  expect_equal(s$rand(), 959852049 /(2**32))
  expect_equal(s$rand(), 2314333085/(2**32))
  expect_equal(s$rand(), 2255782734/(2**32))
  expect_equal(s$rand(), 2921461239/(2**32))
  expect_equal(s$rand(), 1024197102/(2**32))
})

test_that("test_randbelow", {
  s <- SyncRNG(seed=123456)
  expect_equal(s$randbelow(5), 4)
  expect_equal(s$randbelow(5), 0)
  expect_equal(s$randbelow(5), 4)
  expect_equal(s$randbelow(5), 4)
  expect_equal(s$randbelow(5), 2)
})

test_that("test_shuffle", {
  s <- SyncRNG(seed=123456)
  x <- c(1, 2, 3, 4, 5)
  y <- s$shuffle(x)
  expect_equal(y, c(3, 4, 1, 2, 5))
})

test_that("test_first_1000", {
  s <- SyncRNG(seed=0)
  fileName <- "./first_1000_seed_0.txt"
  conn <- file(fileName, open="r")
  linn <- readLines(conn)
  for (i in 1:length(linn)) {
	  exp <- as.numeric(linn[i])
	  expect_equal(exp, s$randi())
  }
  close(conn)
})

printf <- function(...) invisible(cat(sprintf(...)));

test_that("test_first_1000_unif", {
  set.seed(0, 'user', 'user')
  fileName <- "./first_1000_seed_0.txt"
  conn <- file(fileName, open="r")
  linn <- readLines(conn)
  for (i in 1:length(linn)) {
	  exp <- as.numeric(linn[i])
      u <- as.numeric(runif(1)*(2**32 - 1))
	  expect_equal(exp, u)
  }
  close(conn)
})
