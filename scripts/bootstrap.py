'''
Usage:
    python bootstrap.py --target windows
'''

import sys
import os
import argparse
from detail import utils
import multiprocessing

VENDOR_DIR = ".vendor"


def get_project_dir():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    project_dir = os.path.join(script_dir, "..")
    project_dir = os.path.abspath(project_dir)
    return project_dir


def create_vendor_dirs():
    vendor_dir = os.path.join(os.getcwd(), VENDOR_DIR)
    utils.mkdir_p(os.path.join(vendor_dir, "downloads"))
    utils.mkdir_p(os.path.join(vendor_dir, "installed"))
    utils.mkdir_p(os.path.join(vendor_dir, "buildtrees"))


def download_packages():
    dwld_dir = os.path.join(VENDOR_DIR, "downloads")
    with utils.pushd(dwld_dir):
        if not os.path.exists("SourcetrailDB"):
            cmd = "git clone --recursive https://github.com/CoatiSoftware/SourcetrailDB.git"
            utils.execute(cmd.split())


def build_SourcetrailDB_windows(buildtype):
    project_dir = os.getcwd()
    sourcetraildb_dir = os.path.join(VENDOR_DIR, "downloads", "SourcetrailDB")
    sourcetraildb_dir = os.path.join(project_dir, sourcetraildb_dir)
    output_dir = f"windows-{buildtype}"

    install_dir = os.path.join(project_dir, VENDOR_DIR, "installed",
                               output_dir)
    build_dir = os.path.join(project_dir, VENDOR_DIR, "buildtrees",
                             "SourcetrailDB", output_dir)

    utils.mkdir_p(install_dir)
    utils.mkdir_p(build_dir)

    cmake_generate = ["cmake", sourcetraildb_dir]

    cmake_build = ["cmake", "--build", build_dir, "--config", buildtype]

    with utils.pushd(build_dir):
        utils.execute(cmake_generate)
        utils.execute(cmake_build)


def build_packages_for_windows():
    pass
    # build_SourcetrailDB_windows("release")
    # build_SourcetrailDB_windows("debug")


def install_vcpkg_packages():
    vcpkg_dir = os.path.join(get_project_dir(), VENDOR_DIR, "vcpkg")
    if os.path.exists(vcpkg_dir):
        return

    with utils.pushd(os.path.join(get_project_dir(), VENDOR_DIR)):
        cmd = "git clone --recursive https://github.com/Microsoft/vcpkg.git"
        utils.execute(cmd.split())
        with utils.pushd("vcpkg"):
            utils.execute("bootstrap-vcpkg.bat")
            packages = "boost-program-options boost-property-tree"
            vcpkg_cmd = "vcpkg install --triplet x64-windows " + packages
            utils.execute(vcpkg_cmd)


def bootstrap_windows(args):
    project_dir = get_project_dir()
    with (utils.pushd(project_dir)):
        install_vcpkg_packages()
        create_vendor_dirs()
        download_packages()
        build_packages_for_windows()


def bootstrap(args):
    if (args.target == "windows"):
        bootstrap_windows(args)
    else:
        raise NotImplementedError(f"target {args.target} is not supported")


def main(argv):
    raise NotImplementedError("Not yet implemented, please use linux scripts")

    ap = argparse.ArgumentParser()
    ap.add_argument("--target", required=True, help="Target platforms")
    args = ap.parse_args()
    bootstrap(args)


if "__main__" == __name__:
    main(sys.argv)
