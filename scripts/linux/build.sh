#!/bin/bash

set -e

script_dir=$(realpath $(dirname $0))
project_dir=$(realpath ${script_dir}/../..)
cd ${project_dir}

platform=`uname -m`
toolset=gcc.`gcc -dumpversion`
build_type=Release
cores=$(nproc)

if [ ! -z "$1" ]
then
    build_type="$1"
fi

BuildDir="${project_dir}/_build/linux.${platform}.${toolset}.${build_type}"

mkdir -p "${BuildDir}"

cmake \
    ${project_dir} \
    -B${BuildDir} \
    -DCMAKE_BUILD_TYPE=${build_type}

cmake \
    --build ${BuildDir} \
    --config ${build_type} \
    -- -j${cores}
