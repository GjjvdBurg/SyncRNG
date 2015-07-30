library(methods)

dyn.load('RSyncRNG.so')

SyncRNG <- setRefClass('SyncRNG',
	fields=list(
		    seed='numeric',
		    state='numeric'
		    ),
	methods=list(
		initialize=function(..., seed=0) {
			seed <<- seed
			tmp <- .Call('R_syncrng_seed',
			     as.numeric(seed))
			state <<- tmp[1:4]
			callSuper(...)
		},
		randi=function() {
			tmp <- .Call('R_syncrng_rand',
				     as.numeric(state))
			state <<- tmp[1:4]
			return(tmp[5])
		},
		rand=function() {
			r <- randi()
			return (r * 2.3283064365387e-10)
		}
		)
	)
