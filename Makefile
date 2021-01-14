# Makefile for SyncRNG
#
# Author: G.J.J. van den Burg
# Copyright (c) 2021 G.J.J. van den Burg
# License: See LICENSE file
# Date: 2021-01-14
#

SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
MAKEFLAGS += --no-builtin-rules

PYTHON_DIR=./python
R_DIR=./R

.PHONY: all

all: test

###########
# Testing #
###########

.PHONY: test test_R test_python

test: test_R test_python

test_R:
	$(MAKE) -C $(R_DIR) test

test_python:
	$(MAKE) -C $(PYTHON_DIR) test

test_python_direct:
	$(MAKE) -C $(PYTHON_DIR) test_direct

############
# Clean up #
############

.PHONY: clean clean_R clean_python

clean: clean_R clean_python

clean_R:
	$(MAKE) -C $(R_DIR) clean

clean_python:
	$(MAKE) -C $(PYTHON_DIR) clean

