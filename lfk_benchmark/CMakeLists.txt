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

add_library(lfk-core-optimized
        STATIC
        src/core/lfk_core.c)

add_library(lfk-core-nonoptimized
        STATIC
        src/core/lfk_core.c)

if(MSVC)
    string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UPPER)
    string(REGEX REPLACE "(/MDd|/Ob.|/O.|/MD|/RTC.)" "" CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE_UPPER} "${CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE_UPPER}}")
    string(REGEX REPLACE "(/MDd|/Ob.|/O.|/MD|/RTC.)" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

    target_compile_options(lfk-core-optimized    BEFORE PRIVATE /MT /O1 /Ob1 /DNDEBUG)
    target_compile_options(lfk-core-nonoptimized BEFORE PRIVATE /MTd /Zi /Ob0 /Od /RTC1)
else()
    target_compile_options(lfk-core-optimized    BEFORE PRIVATE -O3 -DNDEBUG)
    target_compile_options(lfk-core-nonoptimized BEFORE PRIVATE -O0 -g)
endif()


foreach(OPTIMIZATION optimized nonoptimized)
    set(target lfk-core-${OPTIMIZATION})

    if(MSVC)
        target_compile_definitions(${target}
                        PRIVATE
                        MSVC)
    else()
        target_compile_options(${target} PRIVATE -Wno-error=array-bounds -Wno-array-bounds)
    endif()

    target_include_directories(${target}
            PUBLIC
                inc
            PRIVATE
                src/
            )
    target_compile_definitions(${target}
            PRIVATE
            OPTIMIZATION=${OPTIMIZATION})

endforeach()

if( LFK_WINAPI )
    set(
        PLATFORM_SOURCES
            src/win/platform.c
            src/win/thread.c
    )
else()
    set(
        PLATFORM_SOURCES
            src/posix/platform.c
            src/posix/thread.c
    )
endif()

if(BUILD_SHARED_LIBS)
    add_library(lfk-benchmark
            SHARED
            src/lfk.c
            src/constants.c
            ${PLATFORM_SOURCES}
    )
else()
    add_library(lfk-benchmark
            STATIC
            src/lfk.c
            src/constants.c
            ${PLATFORM_SOURCES}
    )
endif()

target_compile_definitions(lfk-benchmark
        PRIVATE
        COMPILER_INFO="${CMAKE_C_COMPILER_VERSION}"
        PRIVATE
        VERSION="${VERSION}"
        )

target_include_directories(lfk-benchmark
        PUBLIC
            inc/
        PRIVATE
            src/
)

target_link_libraries(lfk-benchmark
        PRIVATE
            lfk-core-optimized
            lfk-core-nonoptimized
        )

if(ANDROID)
    target_link_libraries(lfk-benchmark
            PRIVATE
                m
            )
elseif(LFK_WINAPI)
    if (MSVC)
        target_compile_options(lfk-benchmark PRIVATE /MT)
    endif()
else()
    target_link_libraries(lfk-benchmark
            PRIVATE
                m
                pthread
            )
endif()
