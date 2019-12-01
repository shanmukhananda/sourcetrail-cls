#include "common/pch.hpp"

#include "cls2srctrl/app_entry.hpp"

namespace launcher {
class launcher_raii {
public:
    launcher_raii(int argc, char** argv) : _app_name(argv[0]) {
        std::cout << "Starting:" << _app_name << std::endl;
        std::cout << "Argument count:" << argc << std::endl;
        std::cout << "Arguments are:" << std::endl;
        for (int arg_idx = 0; arg_idx < argc; ++arg_idx) {
            std::cout << argv[arg_idx] << std::endl;
        }
    }
    ~launcher_raii() {
        std::cout << "Exiting:" << _app_name << std::endl;
    }

private:
    std::string _app_name;
};

int run_main(int argc, char** argv,
             std::function<void(int, char**)> app_entry) {
    launcher_raii launch_info_raii(argc, argv);
    auto exit_status{EXIT_SUCCESS};
    try {
        app_entry(argc, argv);
    } catch (const std::exception& ex) {
        std::cerr << argv[0] << ":caught exception:" << ex.what() << std::endl;
        exit_status = EXIT_FAILURE;
    } catch (...) {
        std::cerr << argv[0] << ":caught unknown exception" << std::endl;
        exit_status = EXIT_FAILURE;
    }
    return exit_status;
}
} // namespace launcher

int main(int argc, char** argv) {
    return launcher::run_main(argc, argv, cls2st::run);
}
