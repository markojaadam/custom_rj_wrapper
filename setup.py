from distutils.core import Extension, setup
from Cython.Build import cythonize
from skbuild import setup

# setup(packages=['wrapper', 'primes'])
# ext = Extension(name="messaging", sources=["messaging.pyx"])
# ext = Extension(name="primes", sources=["primes.pyx"], anguage="c++")
setup(packages=['messaging', 'primes'])
# setup(ext_modules=cythonize(ext, compiler_directives={'language_level' : "3"}))

# setup(ext_modules=cythonize(ext))
