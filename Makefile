PYTHON=python2
RLIB=RSyncRNG.so
CFILE=syncrng.c

.PHONY: all clean

all: R python

python:
	$(PYTHON) setup.py build_ext --inplace

R:
	R CMD SHLIB -o $(RLIB) $(CFILE)

clean:
	rm -rf build
	rm -f *.so *.o *.pyc
