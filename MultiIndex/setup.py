from distutils.core import setup
from Cython.Build import cythonize

setup(
    ext_modules = cythonize("inverted_multi_index.pyx")
)