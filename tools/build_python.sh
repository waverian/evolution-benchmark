#!/bin/bash 


PYTHON=python
if [[ "$OSTYPE" == "darwin"* ]]; then
  PYTHON=python3
fi

# let's build evolution-benchmark-lib
cmake -S . -B build_cmake -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
                          -DCMAKE_BUILD_TYPE=Release \
                          -DBUILD_SHARED_LIBS=OFF \
                          -DBUILD_CONSOLE_APP=OFF
cmake --build build_cmake


ROOT_DIR=$PWD
BUILD_DIR=${ROOT_DIR}/build_python/
PY_DIR=${ROOT_DIR}/bindings/python

# remove intermediates
rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

(
  cd ${PY_DIR}

  # setup venv
  ${PYTHON} -m venv ${BUILD_DIR}/temp-venv
  source ${BUILD_DIR}/temp-venv/bin/activate
  ${PYTHON} -m pip install wheel cython==0.29.28 auditwheel

  # setup wheels
  ${PYTHON} -m pip install pip --upgrade
  ${PYTHON} -m pip install setuptools --upgrade
  ${PYTHON} setup.py build_ext -t ${BUILD_DIR} -b ${BUILD_DIR}

  echo "Building wheels"

  pip wheel . -w ${BUILD_DIR}/wheels --cache-dir ${BUILD_DIR}

  LFK_WHEEL=$(ls ${BUILD_DIR}/wheels/)

  mkdir ${BUILD_DIR}/fixed_wheels

  if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    	LD_LIBRARY_PATH=${ROOT_DIR}/build_cmake/benchmark\
    	 auditwheel  repair ${BUILD_DIR}/wheels/$LFK_WHEEL --plat=linux_x86_64 -w ${BUILD_DIR}/fixed_wheels
  elif [[ "$OSTYPE" == "darwin"* ]]; then
      pip install delocate
      DYLD_LIBRARY_PATH=${ROOT_DIR}/build_cmake/benchmark\
       delocate-wheel  -v ${BUILD_DIR}/wheels/$LFK_WHEEL -w ${BUILD_DIR}/fixed_wheels
  fi

  echo
  echo
  echo "wheels available in ${BUILD_DIR}fixed_wheels directory"
  echo $(ls ${BUILD_DIR}/fixed_wheels/)
)
