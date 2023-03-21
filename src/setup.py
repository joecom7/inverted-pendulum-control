from setuptools import setup
from Cython.Build import cythonize

setup(
    name='encoder_utils',
    ext_modules=cythonize("encoder_utils.pyx"),
    zip_safe=False,
)