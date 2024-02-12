# Copyright (c) 2022 waverian. All rights reserved.
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

message(STATUS "Setting up the build options:")

include(CheckCSourceCompiles OPTIONAL RESULT_VARIABLE CheckCSourceCompiles_FOUND)
if(CheckCSourceCompiles_FOUND STREQUAL NOTFOUND)
    set(CheckCSourceCompiles_FOUND OFF)
    message(STATUS "Cannot check source files compiles. Setting defaults")
endif ()

if (CheckCSourceCompiles_FOUND AND NOT DEFINED EB_USE_CPUID )
    file(READ "${CMAKE_HOME_DIRECTORY}/compiler_probes/win_cpuid.c" TEST_SOURCE_CODE)
    check_c_source_compiles("${TEST_SOURCE_CODE}" CAN_USE_WINAPI_CPUID)
    file(READ "${CMAKE_HOME_DIRECTORY}/compiler_probes/posix_cpuid.c" TEST_SOURCE_CODE)
    check_c_source_compiles("${TEST_SOURCE_CODE}" CAN_USE_POSIX_CPUID)

    if(CAN_USE_WINAPI_CPUID)
        set(EB_USE_CPUID WIN)
    elseif (CAN_USE_POSIX_CPUID)
        set(EB_USE_CPUID POSIX)
    else ()
        set(EB_USE_CPUID OFF)
    endif ()
endif ()
if     (${EB_USE_CPUID} STREQUAL WIN)
    add_compile_definitions(EB_USE_WIN_CPUID)
elseif (${EB_USE_CPUID} STREQUAL POSIX)
    add_compile_definitions(EB_USE_POSIX_CPUID)
elseif (${EB_USE_CPUID} STREQUAL OFF)
else ()
    message(FATAL_ERROR "EB_USE_CPUID=${EB_USE_CPUID} is not a valid option. Valid are (WIN,POSIX,OFF)")
endif ()


if (CheckCSourceCompiles_FOUND AND NOT DEFINED EB_USE_THREADS)
    file(READ "${CMAKE_HOME_DIRECTORY}/compiler_probes/win_threads.c" TEST_SOURCE_CODE)
    check_c_source_compiles("${TEST_SOURCE_CODE}" CAN_USE_WIN_THREADS)
    file(READ "${CMAKE_HOME_DIRECTORY}/compiler_probes/posix_threads.c" TEST_SOURCE_CODE)
    check_c_source_compiles("${TEST_SOURCE_CODE}" CAN_USE_POSIX_THREADS)

    if(CAN_USE_WIN_THREADS)
        set(EB_USE_THREADS WIN)
    elseif (CAN_USE_POSIX_THREADS)
        set(EB_USE_THREADS POSIX)
    else ()
        set(EB_USE_THREADS OFF)
    endif ()
endif ()
if     (${EB_USE_THREADS} STREQUAL WIN)
    add_compile_definitions(EB_USE_WIN_THREADS)
elseif (${EB_USE_THREADS} STREQUAL POSIX)
    add_compile_definitions(EB_USE_POSIX_THREADS)
elseif (${EB_USE_THREADS} STREQUAL OFF)
else ()
    message(FATAL_ERROR "EB_USE_THREADS=${EB_USE_THREADS} is not a valid option. Valid are (WIN,POSIX,OFF)")
endif ()

if (CheckCSourceCompiles_FOUND AND NOT DEFINED EB_USE_UNISTD)
    file(READ "${CMAKE_HOME_DIRECTORY}/compiler_probes/unistd.c" TEST_SOURCE_CODE)
    check_c_source_compiles("${TEST_SOURCE_CODE}" CAN_USE_UNISTD)
    if(CAN_USE_UNISTD)
        set(EB_USE_UNISTD ON)
    else ()
        set(EB_USE_UNISTD OFF)
    endif ()
endif ()
if     (${EB_USE_UNISTD} STREQUAL ON)
    add_compile_definitions(EB_USE_UNISTD)
elseif (${EB_USE_UNISTD} STREQUAL OFF)
else ()
    message(FATAL_ERROR "CAN_USE_UNISTD=${CAN_USE_UNISTD} is not a valid option. Valid are (ON,OFF)")
endif ()