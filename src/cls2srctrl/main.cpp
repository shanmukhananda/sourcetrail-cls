#include "common/pch.hpp"

#include "cls2srctrl/cls2srctrl.hpp"
#include "cls2srctrl/config.hpp"

namespace cls2st {
void run(int argc, char** argv) {
    cls2st::config cfg(argc, argv);
    if (cfg.wait_for_key()) {
        std::cout << "press any char and enter to continue\n";
        char x{};
        std::cin >> x;
        std::cout << "received input:" << x << ", continuing...\n";
    }
    cls2st::cls2srctrl app(cfg);
    app.parse_cls();
    app.create_srctrldb();
}
} // namespace cls2st

int main(int argc, char** argv) {
    auto exit_status{EXIT_SUCCESS};
    try {
        cls2st::run(argc, argv);
    } catch (const std::exception& ex) {
        std::cerr << argv[0] << ":caught exception:" << ex.what() << std::endl;
        exit_status = EXIT_FAILURE;
    } catch (...) {
        std::cerr << argv[0] << ":caught unknown exception" << std::endl;
        exit_status = EXIT_FAILURE;
    }
    return exit_status;
}
