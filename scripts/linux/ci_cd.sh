#!/bin/bash

set -e

script_dir=$(realpath $(dirname $0))
project_dir=$(realpath ${script_dir}/../..)
cd ${project_dir}

${project_dir}/scripts/linux/bootstrap.sh
${project_dir}/scripts/linux/build.sh
