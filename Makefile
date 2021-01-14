#
# Makefile for easier installation and cleanup
#
# Uses self-documenting macros from here:
# http://marmelab.com/blog/2016/02/29/auto-documented-makefile.html
#

SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
MAKEFLAGS += --no-builtin-rules

PACKAGE=SyncRNG
VENV_DIR=/tmp/sync_venv/

.PHONY: help

.DEFAULT_GOAL := help

help:
	@grep -E '^[0-9a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) |\
		awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-15s\033[0m\
		%s\n", $$1, $$2}'

################
# Installation #
################

.PHONY: inplace install

inplace:
	python setup.py build_ext --inplace

install: ## Install for the current user using the default python command
	python setup.py build_ext --inplace && \
		python setup.py install --user

################
# Distribution #
################

.PHONY: release dist

release: ## Make a release
	python make_release.py

dist: ## Make Python source distribution
	python setup.py sdist


###########
# Testing #
###########

.PHONY: test

test: venv ## Run nosetests using the default nosetests command
	source $(VENV_DIR)/bin/activate && green -a -vv ./tests

#######################
# Virtual environment #
#######################

.PHONY: venv

venv: $(VENV_DIR)/bin/activate

$(VENV_DIR)/bin/activate:
	test -d $(VENV_DIR) || python -m venv $(VENV_DIR)
	source $(VENV_DIR)/bin/activate && pip install -e .[dev]
	touch $(VENV_DIR)/bin/activate


############
# Clean up #
############

.PHONY: clean

clean: ## Clean build dist and egg directories left after install
	rm -rf ./dist ./build ./$(PACKAGE).egg-info
	rm -rf ./build
	rm -rf ./$(PACKAGE).egg-info
	rm -rf *.so
	rm -f MANIFEST
	rm -f .coverage
	find . -type f -iname '*.pyc' -delete
	find . -type d -name '__pycache__' -empty -delete
