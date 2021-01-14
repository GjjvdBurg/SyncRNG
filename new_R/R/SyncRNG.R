library(methods)

#' A Reference Class for SyncRNG
#' 
#' See \link{syncrng-package} for package documentation.
#'
#' @field seed The seed for the random number generator
#' @field state The current state of the RNG, should not be modified by the 
#' user
#'
#' @useDynLib SyncRNG, .registration = TRUE
#' @export SyncRNG
#' @exportClass SyncRNG
#' @importFrom methods new
#'
#' @examples
#' s <- SyncRNG(seed=123456)
#' for (i in 1:10)
#'   cat(s$randi(), '\n')
#'
SyncRNG <- setRefClass('SyncRNG',
	fields=list(
		    seed='numeric',
		    state='numeric'
		    ),
	methods=list(
		initialize=function(..., seed=0) {
			"Initialize the RNG using the C function R_syncrng_seed"
			seed <<- seed
			tmp <- .Call('R_syncrng_seed',
			     as.numeric(seed))
			state <<- tmp[1:4]
			callSuper(...)
		},
		randi=function() {
			"Generate a single random 32-bit integer"
			tmp <- .Call('R_syncrng_rand',
				     as.numeric(state))
			state <<- tmp[1:4]
			return(tmp[5])
		},
		rand=function() {
			"Generate a single random float in the range [0, 1)"
			r <- randi()
			return (r * 2.3283064365387e-10)
		},
		randbelow=function(n) {
			"Generate a random integer below a given number"
			maxsize <- 2^32
			if (n >= maxsize) {
				warning(paste("Underlying random generator ",
					     "does not supply\n enough bits ",
					     "to choose from a population ",
					     "range this large.\n"))
				return(round(rand() * n))
			}
			rem <- maxsize %% n
			limit <- (maxsize - rem) / maxsize
			r <- rand()
			while (r >= limit)
				r <- rand()
			return(round(r*maxsize) %% n)
		},
		shuffle=function(x) {
			"Randomly shuffle a provided array of values"
			y <- x
			for (i in rev(1:(length(y)-1))) {
				j <- randbelow(i+1)
				tmp <- y[i+1]
				y[i+1] <- y[j+1]
				y[j+1] <- tmp
			}
			return(y)
		}
		)
	)
