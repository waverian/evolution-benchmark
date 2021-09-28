# LFK-MP

### Livermore fortran loops for multicore processors

LFK-MP is a benchmark based on the original [LFK benchmark](https://en.wikipedia.org/wiki/Livermore_loops) by Francis H.
McMahon with support of multicore proccessors.

The benchmark was rewritten in order tu support thread libraries (pthread and winapi threads), with big attention to
match [original benchmark](http://www.netlib.org/benchmark/livermorec) results.

##### Code compiles on the following compilers:

- GCC
- clang
- MinGW
- MSVC
- apple-clang

##### Features:

-- Original 24 kernels.

-- Optimized and Non-Optimized versions of the benchmark in the same binary.

-- No external dependencies.

-- Fancy HTML and Plaintext format report.

## How to run:

```sh
./build_dir/lfk_console --help
Livermore Fortran Loops (LFK) benchmark tool.
Usage:
         -o <file>      ..... report text file                      [default lfk-report.txt]
         -r <file>.html ..... report html file                      [default lfk-report.html]
         -j <cores>     ..... number of cores (0 - auto detection)  [default 1]
         -t <seconds>   ..... execution time in seconds per kernel  [default 1]

```

## License

MIT

#### Special thanks to __Francis H.McMahon__ and __Alexey Eromenko "Technologov"__