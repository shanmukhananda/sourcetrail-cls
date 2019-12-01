#!/bin/bash

set -e

script_dir=$(realpath $(dirname $0))
project_dir=$(realpath ${script_dir}/../..)
cd ${project_dir}
source ${project_dir}/scripts/linux/env.sh

mkdir -p ${results_dir}
pushd ${results_dir} > /dev/null

${install_dir}/bin/sourcetrail-cls.cls2srctrl \
    --cls_input ${project_dir}/data/cls.json \
    --srctrldb_output ${results_dir}/cls.scrtrldb \
    --wait_for_key false

popd > /dev/null
