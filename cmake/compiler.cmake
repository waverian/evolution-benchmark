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

message(STATUS "Configuring compiler:")

set(CMAKE_C_STANDARD 90)

if (NOT MSVC)
    set(CMAKE_C_FLAGS "-fPIC ${CMAKE_C_FLAGS}")
endif ()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "MinSizeRel")
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    if (MSVC)
        # warning level 4
        string(REGEX REPLACE "/W." "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
        set(CMAKE_C_FLAGS "/W4  ${CMAKE_C_FLAGS}")
    else ()
        # lots of warnings and all warnings as errors
        set(CMAKE_C_FLAGS "-Wall -Wextra -Werror ${CMAKE_C_FLAGS} ")
    endif ()
    add_compile_definitions(LFK_RUNFAST)
endif ()

if (MSVC)
    execute_process(COMMAND ${CMAKE_C_COMPILER} ERROR_VARIABLE COMPILER_VERSION OUTPUT_QUIET)
else ()
    execute_process(COMMAND ${CMAKE_C_COMPILER} --version OUTPUT_VARIABLE COMPILER_VERSION)
endif ()
string(REPLACE "\n" ";" COMPILER_VERSION ${COMPILER_VERSION})

list(GET COMPILER_VERSION 0 CMAKE_C_COMPILER_VERSION)

if (NOT ${LFK_TARGET_ARCHITECTURE} STREQUAL "")
    set(CMAKE_C_COMPILER_VERSION "${CMAKE_C_COMPILER_VERSION} ${LFK_TARGET_ARCHITECTURE}")
elseif (NOT ${CMAKE_C_COMPILER_ARCHITECTURE_ID} STREQUAL "")
    set(CMAKE_C_COMPILER_VERSION "${CMAKE_C_COMPILER_VERSION} ${CMAKE_C_COMPILER_ARCHITECTURE_ID}")
endif ()


message(STATUS "Build type: ........... ${CMAKE_BUILD_TYPE}")
message(STATUS "C standard: ........... ${CMAKE_C_STANDARD}")
message(STATUS "Compiler options: ..... ${CMAKE_C_FLAGS}")
message(STATUS "Compiler version: ..... ${CMAKE_C_COMPILER_VERSION}")
