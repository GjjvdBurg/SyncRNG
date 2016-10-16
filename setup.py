"""
    Setup file for SyncRNG

    Author: Gertjan van den Burg
    Date: Oct. 12, 2016
"""


from os import path
from setuptools import setup, find_packages
from distutils.extension import Extension

here = path.abspath(path.dirname(__file__))

with open(path.join(here, 'README.rst'), 'r') as f:
    long_description = f.read()

setup(
        name='SyncRNG',
        author='Gertjan van den Burg',
        author_email='gertjanvandenburg@gmail.com',
        version='1.2.1',
        description='A synchronized Tausworthe RNG for Python and R',
        long_description=long_description,
        url='https://github.com/GjjvdBurg/SyncRNG',
        license='GPL v2',
        packages=find_packages(),
        ext_modules=[
            Extension(
                "syncrng",
                define_macros=[('TARGETPYTHON', '1')],
                sources=["src/syncrng.c"]
                )
            ],
        keywords='RNG R Python',
        classifiers=[
            'Intended Audience :: Developers',
            'Intended Audience :: Science/Research',
            'License :: OSI Approved :: GNU General Public License v2 (GPLv2)',
            'Programming Language :: Python :: 2.7',
            'Programming Language :: Python :: 3',
            'Topic :: Scientific/Engineering :: Mathematics'
            ]
        )
