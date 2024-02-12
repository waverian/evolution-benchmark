# Evolution Benchmark

Waverian Evolution Benchmark is a benchmark based on the original [LFK benchmark](https://en.wikipedia.org/wiki/Livermore_loops) by Francis H.
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

## How to build:

1. Makefile based:
    ```sh
    make
    # Executable located at ./benchmark-cli
    # To disable CPUID run with argument `EB_USE_CPUID=OFF`
    # To disable threads support run with argument `EB_USE_THREADS=OFF`
    ```
2. Cmake based:
    ```sh
    ./cmake_build.sh
    # Executable located at ./benchmark-cli
    # To disable CPUID run with argument `-DEB_USE_CPUID=OFF`
    # To disable threads support run with argument `-DEB_USE_THREADS=OFF`
    ```
3. Cmake based (for windows hosts):
    ```sh
    ./tools/generate_release_win.sh # There might be missing packages. Follow the instructions.
    ```

## How to run:

```sh
Evolution Benchmark tool.
Usage:
         -o <file>    ..... .txt and .html reports file name      [default waverian-benchmark-report]
         -c <comment> ..... comment added to the report
         -j <cores>   ..... number of cores (0 - auto detection)  [default 1]
         -t <seconds> ..... execution time in seconds per kernel  [default 0.5]
         -w           ..... enable workstation mode               [default off]
```

## License

MIT

#### Special thanks to __Francis H.McMahon__ and __Alexey Eromenko "Technologov"__
