#!/bin/bash

set -e

script_dir=$(realpath $(dirname $0))
project_dir=$(realpath ${script_dir}/../..)
cd ${project_dir}
source ${project_dir}/scripts/linux/env.sh

function run_clang_scan_build() {
    echo "Performing scan-build"
    rm -rf ${project_dir}/_out
    scan-build ${project_dir}/scripts/linux/build.sh
}

function run_cppcheck() {
    echo "Performing cppcheck"
    cppcheck \
        --quiet \
        --enable=all \
        --project=${build_dir}/compile_commands.json
}

function run_clang_tidy() {
    echo "Performing clang-tidy"
    cpp_files=$(find ${project_dir}/src -name "*.c" \
        -or -name "*.cc" \
        -or -name "*.cpp")
    for cpp_file in ${cpp_files}
    do
        echo "clang-tidy ${cpp_file}"
        clang-tidy \
            -quiet \
            -checks='*',-fuchsia-default-arguments \
            -p=${build_dir} \
            ${cpp_file}
    done
}

function run_cyclomatic_complexity() {
    echo "Performing cyclomatic complexity check"
    cpp_files=$(find ${project_dir}/src -name "*.c" \
        -or -name "*.cc" \
        -or -name "*.cpp" \
        -or -name "*.h" \
        -or -name "*.hh" \
        -or -name "*.hpp")
    for cpp_file in ${cpp_files}
    do
        echo "pmccabe ${cpp_file}"
        pmccabe -vt \
            ${cpp_file}
    done
}

function run_quality_check() {
    run_clang_scan_build
    run_clang_tidy
    run_cppcheck
    run_cyclomatic_complexity
}

run_quality_check
