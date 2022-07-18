#!/bin/bash
# Copyright (c) 2022 Waverian
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
G_EXECUTABLE=$(basename $0)
G_BASE_DIR=$(dirname $(realpath $0))

source ${G_BASE_DIR}/common.sh


G_BUILD_DIR="build_local"
G_CMAKE_BUILD_DIR="${G_BUILD_DIR}/cmake_build"

function main(){
  for comand in git make cmake gcc
  do
    check_command ${comand} || fail_with "${comand} is missing." \
                                         "Please install git make cmake gcc"
  done

  ls README.md &> /dev/null || fail_with "Please run in the project root. e.g. ./tools/build.sh"

  rm -rf ${G_BUILD_DIR}
  mkdir -p ${G_CMAKE_BUILD_DIR}

  cmake -S $( pwd ) -B ${G_CMAKE_BUILD_DIR}
  cmake --build ${G_CMAKE_BUILD_DIR}
  mv ${G_CMAKE_BUILD_DIR}/lfk_console/lfk-console ${G_BUILD_DIR}/lfk-console

  #rm -rf ${G_CMAKE_BUILD_DIR}
}

main "$@"
