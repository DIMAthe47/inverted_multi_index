from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
from numpy import get_include
import os

# os.environ["CC"] = "g++-6.3"
# os.environ["CXX"] = "g++-6.3"


extensions = [
    Extension("py_inverted_multi_index", ["py_inverted_multi_index.pyx"],
        include_dirs = [r"C:/Users/Dima/GoogleDisk/CLionProjects/image_processing/src",
                        "C:/Users/Dima/Downloads/OpenBLAS-v0.2.19-Win64-int32/OpenBLAS-v0.2.19-Win64-int32/include",
                        get_include()],
        libraries=[r'libopenblas.dll'],
        library_dirs=[r'C:/Users/Dima/Downloads/OpenBLAS-v0.2.19-Win64-int32/OpenBLAS-v0.2.19-Win64-int32/lib']
    )
]


setup(
    name = "build_inverted_multi_index",
    ext_modules = cythonize(extensions),
)

# setup(
#     ext_modules = cythonize("inverted_multi_index.pyx", include_path=[r"C:/Users/Dima/GoogleDisk/CLionProjects/image_processing/src/util"])
# )