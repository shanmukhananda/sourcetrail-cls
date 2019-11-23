import os
from contextlib import contextmanager
import shutil
import sys
import subprocess


@contextmanager
def pushd(destination):
    try:
        cwd = os.getcwd()
        os.chdir(destination)
        yield
    finally:
        os.chdir(cwd)


def mkdir_p(path):
    if not os.path.exists(path):
        os.makedirs(path)


def cmd_exists(cmd):
    if shutil.which(cmd) is None:
        raise FileNotFoundError(f"{cmd} is not installed")


def check_path(path):
    if not os.path.exists(path):
        raise FileNotFoundError(f"{path} does not exist")


def execute(cmd):
    def call(cmd):
        print(f"Starting: {cmd}")
        sys.stdout.flush()
        subprocess.call(cmd)
        print(f"Exited: {cmd}")
        sys.stdout.flush()

    if isinstance(cmd, list):
        call(cmd)
    elif isinstance(cmd, str):
        call(cmd)
    else:
        raise TypeError(f"{cmd} type is invalid")


if "__main__" == __name__:
    pass
