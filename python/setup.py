#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Setup file for SyncRNG

Author: Gertjan van den Burg
Date: Oct. 12, 2016
"""

import io
import os

from setuptools import find_packages, setup
from distutils.extension import Extension

# Package meta-data.
AUTHOR = "Gertjan van den Burg"
DESCRIPTION = "Generate the same random numbers in R and Python"
EMAIL = "gertjanvandenburg@gmail.com"
LICENSE = "GPLv2"
LICENSE_TROVE = "License :: OSI Approved :: GNU General Public License v2 (GPLv2)"
NAME = "SyncRNG"
REQUIRES_PYTHON = ">=3.6.0"
URL = "https://github.com/GjjvdBurg/SyncRNG"
VERSION = None

# What packages are required for this module to be executed?
REQUIRED = []

docs_require = []
test_require = []
dev_require = ["green"]

# What packages are optional?
EXTRAS = {
    "docs": docs_require,
    "test": test_require,
    "dev": docs_require + test_require + dev_require,
}

# The rest you shouldn't have to touch too much :)
# ------------------------------------------------
# Except, perhaps the License and Trove Classifiers!
# If you do change the License, remember to change the Trove Classifier for that!

here = os.path.abspath(os.path.dirname(__file__))

# Import the README and use it as the long-description.
# Note: this will only work if 'README.md' is present in your MANIFEST.in file!
try:
    with io.open(os.path.join(here, "README.md"), encoding="utf-8") as f:
        long_description = "\n" + f.read()
except FileNotFoundError:
    long_description = DESCRIPTION

# Load the package's __version__.py module as a dictionary.
about = {}
if not VERSION:
    project_slug = NAME.replace("-", "_").replace(" ", "_")
    with open(os.path.join(here, project_slug, "__version__.py")) as f:
        exec(f.read(), about)
else:
    about["__version__"] = VERSION

# Where the magic happens:
setup(
    name=NAME,
    version=about["__version__"],
    description=DESCRIPTION,
    long_description=long_description,
    long_description_content_type="text/markdown",
    author=AUTHOR,
    author_email=EMAIL,
    python_requires=REQUIRES_PYTHON,
    url=URL,
    packages=find_packages(
        exclude=["tests", "*.tests", "*.tests.*", "tests.*"]
    ),
    install_requires=REQUIRED,
    extras_require=EXTRAS,
    include_package_data=True,
    license=LICENSE,
    ext_modules=[
            Extension(
                "_syncrng",
                define_macros=[('TARGETPYTHON', '1')],
                sources=["src/_syncrng.c"],
                extra_compile_args=['-g']
                )
    ],
    classifiers=[
        # Trove classifiers
        # Full list: https://pypi.python.org/pypi?%3Aaction=list_classifiers
        LICENSE_TROVE,
            'Intended Audience :: Developers',
            'Intended Audience :: Science/Research',
            'License :: OSI Approved :: GNU General Public License v2 (GPLv2)',
            'Programming Language :: Python :: 3',
            'Topic :: Scientific/Engineering :: Mathematics'
    ],
)
