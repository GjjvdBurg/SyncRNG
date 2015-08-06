
from distutils.core import setup, Extension

setup(
        name='SyncRNG',
        author='Gertjan van den Burg',
        version='1.0',
        description='A synchronized Tausworthe RNG for Python and R',
        license='GPL v2',
        package_dir={'': 'Python'},
        packages=[''],
        ext_modules=[
            Extension(
                "syncrng",
                define_macros=[('TARGETPYTHON', '1')],
                sources=["src/syncrng.c"]
                )
            ],
        )
