#!/bin/bash 

# let's build evolution-benchmark
./cmake_build.sh

ROOT_DIR=$PWD
BUILD_DIR=${ROOT_DIR}/build_python/
PY_DIR=${ROOT_DIR}/python

# remove intermediates
rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

(
  cd ${PY_DIR}

  # setup venv
  python -m venv ${BUILD_DIR}/temp-venv
  source ${BUILD_DIR}/temp-venv/bin/activate
  pip install wheel cython==0.29.28

  # setup wheels
  python setup.py build_ext -t ${BUILD_DIR} -b ${BUILD_DIR}

  echo "Building wheels"

  pip wheel . -w ${BUILD_DIR}/wheels --cache-dir ${BUILD_DIR}

  LFK_WHEEL=$(ls ${BUILD_DIR}/wheels/)

  if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    	LD_LIBRARY_PATH=${ROOT_DIR}/build_cmake/benchmark\
    	 auditwheel  repair ${BUILD_DIR}/wheels/$LFK_WHEEL --plat=linux_x86_64 -w ${BUILD_DIR}/fixed_wheels
  elif [[ "$OSTYPE" == "darwin"* ]]; then
      DYLD_LIBRARY_PATH=${ROOT_DIR}/build_cmake/benchmark\
       delocate-wheel  -v ${BUILD_DIR}/wheels/$LFK_WHEEL -w ${BUILD_DIR}/fixed_wheels
  fi

  echo
  echo
  echo "wheels available in ${BUILD_DIR}fixed_wheels directory"
  echo $(ls ${BUILD_DIR}/fixed_wheels/)
)
