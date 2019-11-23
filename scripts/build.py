'''
Usage:
    python build.py --target windows
'''

import sys
import os
import argparse
from detail import utils
import multiprocessing


def get_project_dir():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    project_dir = os.path.join(script_dir, "..")
    project_dir = os.path.abspath(project_dir)
    return project_dir


def build_windows_impl(buildtype):
    project_dir = get_project_dir()
    build_dir = os.path.join(project_dir, "_build", f"windows-{buildtype}")
    utils.mkdir_p(build_dir)

    with (utils.pushd(build_dir)):
        utils.execute(f"cmake {project_dir}")
        utils.execute(f"cmake --build {build_dir} --config {buildtype}")


def build_windows(args):
    project_dir = get_project_dir()
    with (utils.pushd(project_dir)):
        build_windows_impl("release")
        build_windows_impl("debug")


def build(args):
    if (args.target == "windows"):
        build_windows(args)
    else:
        raise NotImplementedError(f"target {args.target} is not supported")


def main(argv):
    raise NotImplementedError("Not yet implemented, please use linux scripts")

    ap = argparse.ArgumentParser()
    ap.add_argument("--target", required=True, help="Target platforms")
    args = ap.parse_args()
    build(args)


if "__main__" == __name__:
    main(sys.argv)
