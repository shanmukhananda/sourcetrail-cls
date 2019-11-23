#include "common/pch.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " output.srctrldb" << std::endl;
        return EXIT_FAILURE;
    }
    sourcetrail::SourcetrailDBWriter writer;
    writer.open(argv[1]);
    writer.recordSymbol({"::", {{"void", "hello-sourcetraildb", "()"}}});
    writer.close();
    return EXIT_SUCCESS;
}
