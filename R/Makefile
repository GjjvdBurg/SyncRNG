# Makefile for SyncRNG (R package)
#
# Author: G.J.J. van den Burg
# Copyright (c) 2021 G.J.J. van den Burg
# License: See LICENSE file
# Date: 2021-01-14
#

SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
MAKEFLAGS += --no-builtin-rules

PACKAGE=SyncRNG
VERSION=$(shell grep 'Version' ./DESCRIPTION | cut -d':' -f2 | tr -d ' ')

.PHONY: all clean

all: test

############
# Building #
############

.PHONY: build

build: clean
	R CMD build .

###########
# Testing #
###########

.PHONY: check test

test: check

check: build
	R CMD check --as-cran $(PACKAGE)_$(VERSION).tar.gz

############
# Clean up #
############

.PHONY: clean

clean:
	rm -f $(PACKAGE)_*.tar.gz
	rm -rf $(PACKAGE).Rcheck
