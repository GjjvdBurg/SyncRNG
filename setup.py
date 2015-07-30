
from distutils.core import setup, Extension

"""
module1 = Extension('syncrng',
        define_macros = [('TARGETPYTHON', '1')],
        sources=['syncrng.c'])

setup (name = 'SyncRNG',
        version = '0.1',
        description='A synchronized Tausworthe RNG for Python and R',
        ext_modules = [module1])
"""

setup(
        ext_modules=[Extension("syncrng",
            define_macros=[('TARGETPYTHON', '1')],
            sources=["syncrng.c"])],
        )
