import sys
from os import environ
from os.path import dirname, join, abspath

from setuptools import setup, Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

platform = sys.platform

# TODO: take version from VERSION FILE
with open(join(dirname(__file__), 'evolution_benchmark.pyx')) as f:
    for line in f.readline():
        if line.startswith('__version__'):
            __version__ == line.split('=')[-1].strip()[1:-1]

root_dir = abspath(dirname(__file__))

setup(
    name='evolution_benchmark',
    description='Python Interface to Evolution Benchmark',
    author='quanon',
    author_email='akshay@kivy.org',
    cmdclass={'build_ext': build_ext},
    packages=['evolution_benchmark'],
    package_dir={'evolution_benchmark': '.'},
    options={'bdist_wheel':{'universal':'1'}},
    ext_modules=cythonize(
        [Extension(
            "evolution_benchmark",
            [join(root_dir, 'evolution_benchmark.pyx')],
            language="c",
            include_dirs=[join(root_dir, "..", "benchmark", "inc")],
            libraries=["benchmark"],
            library_dirs=[join(root_dir, "..", 'build_cmake', 'benchmark'),
                          join(root_dir, "..", 'build_cmake', 'benchmark', 'Release'),],
            language_level='3',
        )],
        compiler_directives={'language_level' : "3"}
    ),
    version='v1.0.0-Beta.4'
)
