#' @title SyncRNG - Synchronized Random Numbers in R and Python
#'
#' @description 
#' The SyncRNG package provides a random number generator implemented in C and 
#' linked to both R and Python. This way, you can generate the same random 
#' number sequence in both languages by using the same seed.
#'
#' The package implements a Tausworthe LSFR RNG (more details at 
#' \url{https://gertjanvandenburg.com/blog/syncrng/}). This is a very fast 
#' pseudo-random number generator.
#'
#' @section Usage:
#' There are two ways to use this package in R. It can be used as a reference 
#' class, where a SyncRNG object is used to keep the state of the generator and 
#' numbers are generated using the object methods. It can also be used as a 
#' user-defined random number generator using the strategy outlined in 
#' .Random.user. See the examples section below.
#'
#' @author
#' Gerrit J.J. van den Burg\cr
#' Maintainer: Gerrit J.J. van den Burg <gertjanvandenburg@gmail.com>
#'
#' @references
#' URL: \url{https://github.com/GjjvdBurg/SyncRNG}
#'
#' @examples
#' library(SyncRNG)
#'
#' # As user defined RNG:
#'
#' set.seed(0, 'user', 'user')
#' runif(2)
#' # [1] 3.666952e-04 6.257184e-05
#' set.seed(0, 'user', 'user')
#' rnorm(2)
#' # [1] 0.01006027 0.42889422
#'
#' # As class:
#'
#' s <- SyncRNG(seed=0)
#' s$rand()
#' # [1] 0.0003666952
#' s$rand()
#' # [1] 6.257184e-05
#'
#' @name syncrng-package
#' @import methods
#' @keywords internal
"_PACKAGE"
