#!/bin/bash

set -e

script_dir=$(realpath $(dirname $0))
project_dir=$(realpath ${script_dir}/../..)
cd ${project_dir}

platform=`uname -m`
toolset=gcc.`gcc -dumpversion`
build_type=Debug
cores=$(nproc)

build_dir="${project_dir}/_out/build/linux.${platform}.${toolset}.${build_type}"
install_dir="${project_dir}/_out/install/linux.${platform}.${toolset}.${build_type}"
results_dir="${project_dir}/_out/results"
