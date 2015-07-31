#!/bin/bash

paste <(python test.py) <(Rscript test.R)
