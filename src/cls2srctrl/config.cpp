#include "common/pch.hpp"

#include "cls2srctrl/config.hpp"

namespace cls2st {
config::config(const types::cmdline_args_t& arguments) {
    parse(arguments);
}

const types::path_t& config::cls_input() const {
    return _cls_input;
}

const types::path_t& config::srctrldb_output() const {
    return _srctrldb_output;
}

bool config::wait_for_key() const {
    return _wait_for_key;
}

void config::parse(const types::cmdline_args_t& arguments) {
    po::options_description cmd_opts("options");
    // clang-format off
        cmd_opts.add_options()
            ("help", "produce help message")
            ("cls_input", po::value<types::path_t>(&_cls_input)->required(), "input json file in CLS format")
            ("srctrldb_output", po::value<types::path_t>(&_srctrldb_output)->required(), "output .srctrldb file to be generated")
            ("wait_for_key", po::value<bool>(&_wait_for_key), "if true program halts waiting for keyboard input, used to support `Attach to debugger`")
        ;
    // clang-format on
    po::variables_map vm;
    po::store(po::parse_command_line(arguments.size(), arguments.data(),
                                     cmd_opts),
              vm);

    if (vm.count("help") != 0u) {
        std::cout << cmd_opts << std::endl;
        return;
    }

    po::notify(vm);
}
} // namespace cls2st
