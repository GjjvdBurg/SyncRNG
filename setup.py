
from distutils.core import setup, Extension

"""
module1 = Extension('taus',
        define_macros = [('TARGETPYTHON', '1')],
        sources=['taus.c'])

setup (name = 'Tausworthe RNG',
        version = '0.1',
        description='The Tausworthe RNG for Python and R',
        ext_modules = [module1])
"""

setup(
        ext_modules=[Extension("taus",
            define_macros=[('TARGETPYTHON', '1')],
            sources=["taus.c"])],
        )
