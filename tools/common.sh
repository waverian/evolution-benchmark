#!/bin/bash

function fail_with()
{
  printf "\nFATAL: "
  for LINE in "${@}"
  do
    echo "${LINE}" 1>&2
  done
  echo "Terminating ${G_EXECUTABLE}..."
  exit 1
}

function check_command()
{
    printf "Checking %s... " "${1}"
    local EXECUTABLE_PATH=$(which ${1})
    if [[ ${EXECUTABLE_PATH} == "" ]]
    then
      echo "NOT FOUND"
      return 1

    else
      echo "OK"
      return 0
    fi
}

function fetch_previous_version()
{
    local VERSION_TAGS=$(git tag --merged main | grep -P '^v\d+\.\d+\.\d+')
    G_LATEST_VERSION=$(echo "${VERSION_TAGS}" | sort -V | tail -n 1 )
    local VERSION=$(echo "${G_LATEST_VERSION}" | tr -d 'v' | tr '.' ' ' )

    G_VERSION_MAJOR=$(echo "${VERSION}" | awk ' { print $1; } ' )
    G_VERSION_MINOR=$(echo "${VERSION}" | awk ' { print $2; } ' )
    G_VERSION_PATCH=$(echo "${VERSION}" | awk ' { print $3; } ' )
}

function fetch_version_from_file()
{
    local VERSION=$(cat VERSION | tr -d 'v' | tr '.' ' ' )

    G_VERSION_MAJOR=$(echo "${VERSION}" | awk ' { print $1; } ' )
    G_VERSION_MINOR=$(echo "${VERSION}" | awk ' { print $2; } ' )
    G_VERSION_PATCH=$(echo "${VERSION}" | awk ' { print $3; } ' )
}

function bump_version()
{
  case $1 in
    MAJOR)
      ((G_VERSION_MAJOR++))
      G_VERSION_MINOR=0
      G_VERSION_PATCH=0
      ;;
    MINOR)
      ((G_VERSION_MINOR++))
      G_VERSION_PATCH=0
      ;;
    PATCH)
      ((G_VERSION_PATCH++))
      ;;
    *)
      fail_with "$0 called with wrong parameter $1"
      ;;
  esac
}

function generate_version()
{
  echo "${G_VERSION_MAJOR}.${G_VERSION_MINOR}.${G_VERSION_PATCH}" > VERSION
  git add VERSION && git commit --amend --no-edit
  GIT_TAG="v${G_VERSION_MAJOR}.${G_VERSION_MINOR}.${G_VERSION_PATCH}"
  git tag -d ${GIT_TAG}
  git tag ${GIT_TAG} || fail_with "Cannot create tag for version ${GIT_TAG}"
}

function build() {
  local BUILD_DIR=${G_RELEASE_DIR}/$1
  local CC=$2
  shift && shift

  mkdir -p "${BUILD_DIR}" || fail_with "Cannot create build folder ${BUILD_DIR}"

  cmake -DCMAKE_C_COMPILER=${CC}      \
        -DCMAKE_C_COMPILER_WORKS=1    \
        -S .                          \
        -B ${BUILD_DIR}               \
        $@                            \
        &&
  cmake --build ${BUILD_DIR} -- -j3   \
        || fail_with "The build for ${BUILD_DIR} is failed"

  local EXECUTABLE_NAME=$(echo "lfk-benchmark_${G_VERSION_MAJOR}-${G_VERSION_MINOR}-${G_VERSION_PATCH}_$(basename ${BUILD_DIR})" | tr '/' '-' )
  cp ${BUILD_DIR}/lfk_console/lfk-console     ${G_RELEASE_DIR}/${EXECUTABLE_NAME}     2> /dev/null || \
  cp ${BUILD_DIR}/lfk_console/lfk-console.exe ${G_RELEASE_DIR}/${EXECUTABLE_NAME}.exe 2> /dev/null || \
  fail_with "lfk-console./ge  /lfk-console executable was not created"

  rm -rf ${BUILD_DIR}
}
function fetch_current_version() {
    PENDING_CHANGES=$(git log --pretty="%s" ${G_LATEST_VERSION}..HEAD | tac)
    IFS=$'\n'
    for LINE in ${PENDING_CHANGES}
    do
      if [[  $LINE == feat!* ]]
      then
        echo "${LINE} MAJOR"
        bump_version MAJOR
      elif [[ $LINE == feat* ]]
      then
        echo "${LINE} MINOR"
        bump_version MINOR
      else
        echo "${LINE} PATCH"
        bump_version PATCH
      fi
    done
    unset IFS

   echo "VERSION  ${G_VERSION_MAJOR}.${G_VERSION_MINOR}.${G_VERSION_PATCH}"
}

function generate_changelog() {
    GIT_VERSION_TAGS=$(git tag --merged main | grep -P '^v\d+\.\d+\.\d+' | sort -Vr && git rev-list HEAD | tail -n 1 )

    touch ${G_RELEASE_DIR}/CHANGELOG

    for GIT_CURRENT_TAG in ${GIT_VERSION_TAGS}
    do
          if [[ ${GIT_PREVIOUS_TAG} != "" ]]
          then
            (
              git show --quiet ${GIT_PREVIOUS_TAG} --date=short --pretty="%cd" | tr -d '\n'
              echo " | Release ${GIT_PREVIOUS_TAG}"
              git log ${GIT_CURRENT_TAG}..${GIT_PREVIOUS_TAG} --pretty="%B" | sed -e 's/^/\t/'
            ) >> ${G_RELEASE_DIR}/CHANGELOG
          fi
          GIT_PREVIOUS_TAG=${GIT_CURRENT_TAG}
    done
}

