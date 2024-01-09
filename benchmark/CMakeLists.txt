# Copyright (c) 2022 waverian
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

add_library(benchmark-core-optimized
        STATIC
        src/core/core.c)

add_library(benchmark-core-nonoptimized
        STATIC
        src/core/core.c)

if (MSVC)
    string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UPPER)
    string(REGEX REPLACE "(/MDd|/Ob.|/O.|/MD|/RTC.)" "" CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE_UPPER} "${CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE_UPPER}}")
    string(REGEX REPLACE "(/MDd|/Ob.|/O.|/MD|/RTC.)" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

    target_compile_options(benchmark-core-optimized BEFORE PRIVATE /MT /O1 /Ob1 /DNDEBUG)
    target_compile_options(benchmark-core-nonoptimized BEFORE PRIVATE /MTd /Zi /Ob0 /Od /RTC1)
else ()
    target_compile_options(benchmark-core-optimized BEFORE PRIVATE -O3 -DNDEBUG)
    target_compile_options(benchmark-core-nonoptimized BEFORE PRIVATE -O0 -g)
endif ()


foreach (OPTIMIZATION optimized nonoptimized)
    set(target benchmark-core-${OPTIMIZATION})

    if (MSVC)
        target_compile_definitions(${target}
                PRIVATE
                MSVC)
    else ()
        target_compile_options(${target} PRIVATE -Wno-error=array-bounds -Wno-array-bounds)
    endif ()

    target_include_directories(${target}
            PUBLIC
            inc
            PRIVATE
            src/
    )
    target_compile_definitions(${target}
            PRIVATE
            OPTIMIZATION=${OPTIMIZATION})

endforeach ()

set(BENCHMARK_SOURCES
        src/benchmark.c
        src/constants.c
        src/platform.c
        src/thread.c
        src/printers/html.c
        src/printers/text.c)
if (BUILD_SHARED_LIBS)
    add_library(benchmark
            SHARED
            ${BENCHMARK_SOURCES}
    )
else ()
    add_library(benchmark
            STATIC
            ${BENCHMARK_SOURCES}
    )
endif ()

target_include_directories(benchmark
        PUBLIC
        inc/
        PRIVATE
        src/
)

target_link_libraries(benchmark
        PRIVATE
        benchmark-core-optimized
        benchmark-core-nonoptimized
)

if (ANDROID)
    target_link_libraries(benchmark
            PRIVATE
            m
    )
elseif (WB_USE_WINAPI)
    if (MSVC)
        target_compile_options(benchmark PRIVATE /MT)
    endif ()
else ()
    target_link_libraries(benchmark
            PRIVATE
            m
            pthread
    )
endif ()