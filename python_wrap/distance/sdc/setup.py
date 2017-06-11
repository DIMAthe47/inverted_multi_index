from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
from numpy import get_include
import os


extensions = [
    Extension("py_symmetric_distance_computer", ["py_symmetric_distance_computer.pyx"],
              include_dirs=[r"C:/Users/Dima/GoogleDisk/CLionProjects/image_processing/src",
                            get_include()]
              )
]

setup(
    name="py_symmetric_distance_computer",
    ext_modules=cythonize(extensions),
)