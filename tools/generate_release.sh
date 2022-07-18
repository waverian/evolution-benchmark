#!/bin/bash
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
G_EXECUTABLE=$(basename $0)
G_BASE_DIR=$(dirname $(realpath $0))

source ${G_BASE_DIR}/common.sh

G_RELEASE_DIR="release"

G_LATEST_VERSION="0.0.0"
G_VERSION_MAJOR=0
G_VERSION_MINOR=0
G_VERSION_PATCH=0

function main(){
  for comand in zip git cc make cmake x86_64-w64-mingw32-gcc-win32 i686-w64-mingw32-gcc-win32
  do
    check_command ${comand} || fail_with "${comand} is missing." \
                                         "Please install zip git make cmake  gcc gcc-multilib mingw-w64 packages"
  done

  ls README.md &>/dev/null || fail_with "Please run in the project root. E.g. ./tools/generate_release.sh"

  rm -rf ${G_RELEASE_DIR}
  mkdir -p ${G_RELEASE_DIR}

  fetch_version_from_file

  build "linux_x86"      gcc                                       \
                             -DCMAKE_C_FLAGS="-m32"                \
                             -DLFK_TARGET_ARCHITECTURE="32-bit"

  build "linux_x86-64"   gcc                                       \
                             -DCMAKE_C_FLAGS="-m64"                \
                             -DLFK_TARGET_ARCHITECTURE="64-bit"

  build "windows_x86"    i686-w64-mingw32-gcc-win32                \
                            -DCMAKE_SYSTEM_NAME="Windows"          \
                            -DLFK_TARGET_ARCHITECTURE="32-bit"

  build "windows_x86-64" x86_64-w64-mingw32-gcc-win32              \
                            -DCMAKE_SYSTEM_NAME="Windows"          \
                            -DLFK_TARGET_ARCHITECTURE="64-bit"


  RELEASE_NAME="lfk-mp-v${G_VERSION_MAJOR}.${G_VERSION_MINOR}.${G_VERSION_PATCH}"

  zip -r ${G_RELEASE_DIR}/${RELEASE_NAME}-src.zip $(git ls-tree main --name-only -r)
  ( cd ${G_RELEASE_DIR} && zip -r ../${RELEASE_NAME}.zip ./* )
}

main "$@"


