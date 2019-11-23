import utils
import subprocess
import os
import sys


def get_project_dir():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    project_dir = os.path.join(script_dir, "..", "..")
    project_dir = os.path.abspath(project_dir)
    return project_dir


def main(argv):
    project_dir = get_project_dir()
    with utils.pushd(project_dir):
        test_installation = os.path.join(project_dir, "scripts", "detail",
                                         "test_installation.py")
        command = f"python {test_installation} --target windows"
        utils.execute(command)

        bootstrap = os.path.join(project_dir, "scripts", "bootstrap.py")
        command = f"python {bootstrap} --target windows"
        utils.execute(command)

        build = os.path.join(project_dir, "scripts", "build.py")
        command = f"python {build} --target windows"
        utils.execute(command)


if "__main__" == __name__:
    main(sys.argv)
