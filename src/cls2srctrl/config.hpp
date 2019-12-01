#pragma once

#include "common/pch.hpp"

#include "cls2srctrl/types.hpp"

namespace po = boost::program_options;

namespace cls2st {
class config {
public:
    config(int, char**);
    const types::path_t& cls_input() const;
    const types::path_t& srctrldb_output() const;
    bool wait_for_key() const;

private:
    void parse(int, char**);
    types::path_t _cls_input{};
    types::path_t _srctrldb_output{};
    bool _wait_for_key{false};
};
} // namespace cls2st
