#!/bin/bash

set -e

script_dir=$(realpath $(dirname $0))
project_dir=$(realpath ${script_dir}/../..)
cd ${project_dir}
source ${project_dir}/scripts/linux/env.sh

${project_dir}/scripts/linux/bootstrap.sh
${project_dir}/scripts/linux/build.sh
${project_dir}/scripts/linux/quality.sh
