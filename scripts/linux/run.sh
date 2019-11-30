
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

install_dir="${project_dir}/_out/install/linux.${platform}.${toolset}.${build_type}"
results_dir="${project_dir}/_out/results"

mkdir -p ${results_dir}
pushd ${results_dir} > /dev/null

${install_dir}/bin/sourcetrail-cls.cls2srctrl \
    --cls_input ${project_dir}/data/cls.json \
    --srctrldb_output ${results_dir}/cls.scrtrldb \
    --wait_for_key true

popd > /dev/null
