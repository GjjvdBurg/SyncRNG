
from setuptools import setup, find_packages
from distutils.extension import Extension

setup(
        name='SyncRNG',
        author='Gertjan van den Burg',
        version='1.1.0',
        description='A synchronized Tausworthe RNG for Python and R',
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
            'Indended Audience :: Science/Research',
            'License :: OSI Approved :: GNU General Public License v2 (GPLv2)',
            'Programming Language :: Python :: 2.7',
            'Programming Language :: Python :: 3',
            'Topic :: Scientific/Engineering :: Mathematics'
            ]
        )
