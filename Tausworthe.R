library(methods)

dyn.load('tausR.so')

TauswortheRNG <- setRefClass('TauswortheRNG',
	fields=list(
		    seed='numeric',
		    state='numeric'
		    ),
	methods=list(
		initialize=function(..., seed=0) {
			seed <<- seed
			tmp <- .Call('R_tausworthe_seed',
			     as.integer(seed))
			state <<- tmp[1:4]
			callSuper(...)
		},
		randi=function() {
			tmp <- .Call('R_tausworthe_rand',
				     as.integer(state))
			state <<- tmp[1:4]
			return(tmp[5])
		},
		rand=function() {
			r <- randi()
			return (r * 2.3283064365387e-10)
		}
		)
	)

taus.seed <- function(seed=0)
{
	t <- TauswortheRNG(seed=seed)
	return(t)
}

taus.rand <- function(t)
{
	return(t$rand())
}

taus.randi <- function(t)
{
	return(t$randi())
}
