PYTHON=python2

.PHONY: all clean

all: R python

python:
	$(PYTHON) setup.py build_ext --inplace

R:
	R CMD SHLIB -o tausR.so taus.c

clean:
	rm -rf build
	rm -f taus.so
	rm -f tausR.so
	rm -f taus.o
