#pragma once

#include "common/pch.hpp"

namespace po = boost::program_options;
namespace fs = std::experimental::filesystem;

namespace cls2st {
class config {
public:
    config(int argc, char** argv) {
        parse(argc, argv);
    }
    const fs::path& cls_input() const {
        return _cls_input;
    }
    const fs::path& srctrldb_output() const {
        return _srctrldb_output;
    }
    bool wait_for_key() const {
        return _wait_for_key;
    }
private:
    void parse(int argc, char** argv) {
        po::options_description cmd_opts("options");
        // clang-format off
        cmd_opts.add_options()
            ("help", "produce help message")
            ("cls_input", po::value<fs::path>(&_cls_input)->required(), "input json file in CLS format")
            ("srctrldb_output", po::value<fs::path>(&_srctrldb_output)->required(), "output .srctrldb file to be generated")
            ("wait_for_key", po::value<bool>(&_wait_for_key), "if true program halts waiting for keyboard input, used to support `Attach to debugger`")
        ;
        // clang-format on
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, cmd_opts), vm);

        if (vm.count("help")) {
            std::cout << cmd_opts << std::endl;
            return;
        }

        po::notify(vm);
    }
    fs::path _cls_input{};
    fs::path _srctrldb_output{};
    bool _wait_for_key{false};
};
} // namespace cls2st
