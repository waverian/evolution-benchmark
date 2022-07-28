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

message(STATUS "Setting up the definitions:")

if (MSVC)
    set(LFK_WINAPI ON)
    add_compile_definitions(LFK_WINAPI)
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(LFK_WINAPI ON)
    add_compile_definitions(LFK_WINAPI)
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    set(LFK_WINAPI OFF)
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    set(LFK_WINAPI OFF)
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fembed-bitcode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fembed-bitcode")
    set(CMAKE_XCODE_ATTRIBUTE_ARCHS[sdk=iphoneos*] "${__iphoneos_archs}")
    #set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator15.4.sdk")
    set(LFK_WINAPI OFF)
else ()
    message(FATAL "Unsupported target host system name ${CMAKE_SYSTEM_NAME}")
endif ()

message(STATUS "CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}")
if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "^(x86_64|x86|AMD64)$" AND NOT ("${CMAKE_OSX_ARCHITECTURES}" MATCHES "arm"))
    set(LFK_USE_CPUID ON)
    add_compile_definitions(LFK_USE_CPUID)
endif ()

add_compile_definitions(LFK_VERSION="${LFK_VERSION}")
add_compile_definitions(LFK_COMPILER="${CMAKE_C_COMPILER_VERSION}")

#USE_POSIX