
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

mkdir -p ${project_dir}/_tmp
pushd ${project_dir}/_tmp > /dev/null

${build_dir}/src/cls2srctrl/sourcetrail-cls.cls2srctrl \
    --cls_input ${project_dir}/data/cls.json \
    --srctrldb_output ${project_dir}/_tmp/cls.scrtrldb \
    --wait_for_key false

popd > /dev/null

