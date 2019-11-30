#!/bin/bash

set -e

script_dir=$(realpath $(dirname $0))
project_dir=$(realpath ${script_dir}/../..)
cd ${project_dir}

function format_cpp_files() {
    cpp_files=$(find -name "*.c" \
        -or -name "*.c" \
        -or -name "*.cc" \
        -or -name "*.cpp" \
        -or -name "*.h" \
        -or -name "*.hh" \
        -or -name "*.hpp")
    for cpp_file in ${cpp_files}
    do
        echo "Formating ${cpp_file}"
        clang-format -i -style=file ${cpp_file}
    done
}

function format_cmake() {
    cmake_files=$(find -name "CMakeLists.txt")
    for cmake_file in ${cmake_files}
    do
        echo "Formating ${cmake_file}"
        cmake-format -i ${cmake_file}
    done
}

function format() {
    format_cmake
    format_cpp_files
}

format
