import utils
import subprocess
import os
import sys


def get_project_dir():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    project_dir = os.path.join(script_dir, "..", "..")
    project_dir = os.path.abspath(project_dir)
    return project_dir


def code_format(directory):
    for root, dirs, files in os.walk(directory):
        for filename in files:
            if filename.endswith(('.cpp', '.hpp', '.cc', '.hh', '.c', '.h')):
                filepath = os.path.join(root, filename)
                format_cmd = f"clang-format -i -style=file {filepath}"
                utils.execute(format_cmd.split())
            elif filename.endswith(('.py')):
                filepath = os.path.join(root, filename)
                format_cmd = f"yapf -i {filepath}"
                utils.execute(format_cmd.split())


def main(argv):
    project_dir = get_project_dir()

    utils.cmd_exists("clang-format")
    utils.cmd_exists("yapf")

    with utils.pushd(project_dir):
        code_format(os.path.join(project_dir, "src"))
        code_format(os.path.join(project_dir, "scripts"))


if "__main__" == __name__:
    main(sys.argv)
