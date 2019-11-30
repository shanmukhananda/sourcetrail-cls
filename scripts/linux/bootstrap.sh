#!/bin/bash

set -e

if [ ! "${EUID}" = "0" ]
then
   echo "ERROR: ${0} must be run as root" 
   exit 1
fi

cores=$(nproc)
packages_dir=/root/packages
install_dir=/usr/local
mkdir -p ${packages_dir}
build_type=Release

function install_dependencies() {
    apt-get update --fix-missing
    apt-get install -y \
        apt-utils \
        build-essential \
        cmake \
        git \
        libboost-all-dev \
        rsync \
        wget \
        zip
}

function download_file() {
    cd ${packages_dir}
    folder=${packages_dir}/${1}
    url=${2}

    mkdir -p ${folder}
    cd ${folder}
    wget -N ${url}
    cd ${packages_dir}
}

function download_packages() {
    srctraildb_url=https://github.com/CoatiSoftware/SourcetrailDB/archive/master.tar.gz
    download_file SourcetrailDB ${srctraildb_url}
}

function install_SourcetrailDB() {
    cd ${packages_dir}/SourcetrailDB
    extracted_folder=SourcetrailDB-master
    archive_file=master.tar.gz
    if [ ! -e ${extracted_folder} ]
    then
        mkdir -p ${extracted_folder}
        tar xzf ${archive_file} --directory=${extracted_folder} --strip-components=1
    fi
    echo ${extracted_folder}
    cd ${extracted_folder}
    mkdir -p ${build_type}
    cd ${build_type}
    cmake -D CMAKE_BUILD_TYPE=${build_type} -D CMAKE_INSTALL_PREFIX=${install_dir} ..
    make -j${cores}
    mkdir -p /usr/local/include/SourcetrailDB
    cp -rf ${packages_dir}/SourcetrailDB/${extracted_folder}/core/include/* /usr/local/include/SourcetrailDB
    cp -rf ./core/libsourcetraildb.a /usr/local/lib
    # make install
}

install_dependencies
download_packages
install_SourcetrailDB
