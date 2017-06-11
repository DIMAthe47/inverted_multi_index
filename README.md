# inverted_multi_index
Set of modules with primary purpose to work with inverted multi-index.

It includes:
- Euclidean distance computation
- finding nearest indices by given distances
- building inverted multi-index
- multi-sequence algorithm

There are also:
- symmetric distance computation(Euclidean)
- asymmetric distance computation(Euclidean)

which are used in nearest neighbor search with quantization approach.

For Euclidean distances computatation OpenBlas library is used. Setting up library location in CMakeLists.txt required.

There are also [Cython wraps](python_wrap) for major functionalities to work with in Python.
