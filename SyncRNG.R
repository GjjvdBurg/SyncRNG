library(methods)

frame.files <- lapply(sys.frames(), function(x) x$ofile)
frame.files <- Filter(Negate(is.null), frame.files)

script.dir <- normalizePath(dirname(frame.files[[length(frame.files)]]))
source.file <- paste(script.dir, '/', 'RSyncRNG.so', sep='')

dyn.load(source.file)

SyncRNG <- setRefClass('SyncRNG',
	fields=list(
		    seed='numeric',
		    state='numeric',
		    BPF='numeric'
		    ),
	methods=list(
		initialize=function(..., seed=0) {
			BPF <<- 32
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
		},
		randbelow=function(n) {
			maxsize <- 2^BPF
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
