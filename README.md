# sourcetrail-cls

**Generate Sourcetrail project for CLS (Common Language Syntax)**

## Build Status

| Linux                                               |
|-----------------------------------------------------|
| [![Build Status][appveyor_status]][appveyor_builds] |

[appveyor_status]: https://ci.appveyor.com/api/projects/status/x9rv935g9ki26i9f?svg=true
[appveyor_builds]: https://ci.appveyor.com/project/shanmukhananda/sourcetrail-cls

## Demo

Sourcetrial project created from [CLS JSON file](data/cls.json)
![Alt Text](docs/demo.gif)

## Installation

Refer [Installation](docs/Installation.md)

## Build

- `scripts/linux/bootstrap.sh`
- `scripts/linux/build.sh`

## Usage

- `scripts/linux/run.sh`

Modify `run.sh` according to your need

## Known Issues

- Ouput file will always be `project.srctrldb`
    - Original idea is to parametrize output file by taking the output path as argument
    - If you run the app `sourcetrail-cls.cls2srctrl`, parameter `--srctrldb_output` has no effect 
    - It is possible to create `.srctrldb` file using this idea. But the created file fails to load in Sourcetrail. This needs to be investigated.
    - Output path is hardcoded in file, `src/cls2srctrl/cls2srctr.hpp`, function `cls2st::cls2srctrl::create_srctrldb`
