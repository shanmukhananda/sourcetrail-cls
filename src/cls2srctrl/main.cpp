#include "common/pch.hpp"

#include "cls2srctrl/app_entry.hpp"

// @todo: move launcher to a common reusable place
namespace launcher {
class launcher_raii {
public:
    explicit launcher_raii(const cls2st::types::cmdline_args_t& arguments)
        : _app_name(arguments[0]) {
        std::cout << "Starting:" << _app_name << std::endl;
        std::cout << "Argument count:" << arguments.size() << std::endl;
        std::cout << "Arguments are:" << std::endl;
        for (const auto& args : arguments) {
            std::cout << args << std::endl;
        }
    }

    ~launcher_raii() {
        std::cout << "Exiting:" << _app_name << std::endl;
    }

    launcher_raii(const launcher_raii&) = delete;
    launcher_raii& operator=(const launcher_raii&) = delete;
    launcher_raii(launcher_raii&&) = delete;
    launcher_raii& operator=(launcher_raii&&) = delete;

private:
    std::string _app_name;
};

int run_main(const cls2st::types::entry_function_t& app_entry,
             const cls2st::types::cmdline_args_t& arguments) {
    launcher_raii launch_info_raii(arguments);
    auto exit_status{EXIT_SUCCESS};
    try {
        app_entry(arguments);
    } catch (const std::exception& ex) {
        std::cerr << arguments[0] << ":caught exception:" << ex.what()
                  << std::endl;
        exit_status = EXIT_FAILURE;
    } catch (...) {
        std::cerr << arguments[0] << ":caught unknown exception" << std::endl;
        exit_status = EXIT_FAILURE;
    }
    return exit_status;
}
} // namespace launcher

int main(int argc, char** argv) {
    const cls2st::types::cmdline_args_t arguments{argv, argc};
    return launcher::run_main(cls2st::run, arguments);
}
