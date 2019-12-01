#!/bin/bash

set -e

script_dir=$(realpath $(dirname $0))
project_dir=$(realpath ${script_dir}/../..)
cd ${project_dir}
source ${project_dir}/scripts/linux/env.sh

mkdir -p "${build_dir}"
mkdir -p "${install_dir}"

cmake \
	-G "Unix Makefiles" \
    ${project_dir} \
    -B${build_dir} \
    -DCMAKE_BUILD_TYPE=${build_type} \
	-DCMAKE_INSTALL_PREFIX=${install_dir}

cmake \
    --build ${build_dir} \
    --config ${build_type} \
    --target install \
    -- -j${cores}
