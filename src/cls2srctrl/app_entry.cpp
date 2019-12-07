#include "common/pch.hpp"

#include "cls2srctrl/app_entry.hpp"
#include "cls2srctrl/cls2srctrl.hpp"
#include "cls2srctrl/config.hpp"
#include "cls2srctrl/types.hpp"

namespace cls2st {
void wait_for_key() {
    char x{'0'};
    std::cout << "Press any char and enter to continue!" << std::endl;
    std::cin >> x;
    std::cout << "Received key input " << x << " continuing..." << std::endl;
}

void run(const types::cmdline_args_t& arguments) {
    cls2st::config cfg(arguments);
    if (cfg.wait_for_key()) {
        wait_for_key();
    }
    cls2st::cls2srctrl app(cfg);
    app.parse_cls();
    app.create_srctrldb();
}
} // namespace cls2st
