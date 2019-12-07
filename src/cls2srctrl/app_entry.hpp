#pragma once

#include "common/pch.hpp"

#include "cls2srctrl/types.hpp"

namespace cls2st {
void wait_for_key();
void run(const types::cmdline_args_t&);
} // namespace cls2st
