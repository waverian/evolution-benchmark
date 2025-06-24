from os.path import dirname, join, abspath
from setuptools import setup, Extension
from Cython.Build import cythonize

root_dir = abspath(dirname(__file__))

try:
    from Cython.Distutils import build_ext
    source_file = 'evolution_benchmark.pyx'
    cmdclass = {'build_ext': build_ext}
except ImportError:
    source_file = 'evolution_benchmark.c'
    cmdclass = {}

with open(join(root_dir, '..', '..', 'VERSION')) as f:
    __version__ = f.readline()

extensions = Extension(
        name = 'evolution_benchmark',
        sources = [join(root_dir, source_file)],
        language='c',
        include_dirs=[join(root_dir, '..', '..', 'lib', 'inc')],
        libraries=['evolution-benchmark'],
        library_dirs=[
            join(root_dir, '..', '..', 'build_cmake', 'lib'),
            join(root_dir, '..', '..', 'build_cmake', 'lib', 'Release'),
        ])

cython_directives = {'language_level': "3"}
setup(
    name='evolution_benchmark',
    description='Python Interface to Waverian Evolution Benchmark',
    author='Pavlo Syrykh',
    author_email='psyrykh@gmail.com',
    cmdclass=cmdclass,
    packages=['evolution_benchmark'],
    package_dir={'evolution_benchmark': '.'},
    options={
        'bdist_wheel':{'universal':'1'}
    },
    ext_modules= cythonize(extensions, compiler_directives=cython_directives),
    version=__version__
)
