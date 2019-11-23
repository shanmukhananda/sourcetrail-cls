import utils
import os
import sys
import argparse


def test_common_targets(args):
    utils.cmd_exists("git")
    utils.cmd_exists("cmake")
    utils.cmd_exists("clang-format")
    utils.cmd_exists("conda")
    utils.cmd_exists("python")


def test_windows_target(args):
    pass


def test_installation(args):
    test_common_targets(args)
    if (args.target == "windows"):
        test_windows_target(args)
    else:
        raise NotImplementedError(f"target {args.target} is not supported")


def main(argv):
    ap = argparse.ArgumentParser()
    ap.add_argument("--target", required=True, help="Target platforms")
    args = ap.parse_args()
    test_installation(args)


if "__main__" == __name__:
    main(sys.argv)
