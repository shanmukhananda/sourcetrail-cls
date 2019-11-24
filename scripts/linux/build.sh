#!/bin/bash

set -e

script_dir=$(realpath $(dirname $0))
project_dir=$(realpath ${script_dir}/../..)
cd ${project_dir}

platform=`uname -m`
toolset=gcc.`gcc -dumpversion`
build_type=Debug
cores=$(nproc)

if [ ! -z "$1" ]
then
    build_type="$1"
fi

build_dir="${project_dir}/_build/linux.${platform}.${toolset}.${build_type}"

mkdir -p "${build_dir}"

cmake \
    ${project_dir} \
    -B${build_dir} \
    -DCMAKE_BUILD_TYPE=${build_type}

cmake \
    --build ${build_dir} \
    --config ${build_type} \
    -- -j${cores}
