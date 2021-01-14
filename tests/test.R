library(SyncRNG)

test.randi <- function()
{
	s <- SyncRNG(seed=123456)
	for (i in 1:5)
		cat(s$randi(), '\n')
}

test.rand <- function()
{
	s <- SyncRNG(seed=123456)
	for (i in 1:5)
		cat(sprintf('%.16f\n', s$rand()))
}

test.randbelow <- function()
{
	s <- SyncRNG(seed=123456)
	for (i in 1:5)
		cat(s$randbelow(i), '\n')
}

test.shuffle <- function()
{
	s <- SyncRNG(seed=123456)
	x <- c(1:5)
	for (i in 1:5) {
		y <- s$shuffle(x)
		x <- y
		cat('[', paste(y, collapse=', '), ']\n', sep='')
	}
}

main <- function()
{
	test.randi()
	test.rand()
	test.randbelow()
	test.shuffle()
}

main()
