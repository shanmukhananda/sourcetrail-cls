#include "SourcetrailDB/SourcetrailDBWriter.h"

#include <iostream>
#include <assert.h>

void throw_on_error(const sourcetrail::SourcetrailDBWriter& writer, bool is_valid) {
    if(!is_valid) {
        std::cout << "Error: " << writer.getLastError() << std::endl;
        throw std::runtime_error(writer.getLastError());
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " output.srctrldb" << std::endl;
        return EXIT_FAILURE;
    }
    sourcetrail::SourcetrailDBWriter writer;

    bool is_valid{false};

    std::cout << "Opening writer " << argv[1] << std::endl;
    is_valid = writer.open(argv[1]);
    throw_on_error(writer, is_valid);

    std::cout << "Recording symbol" << std::endl;

    auto id = writer.recordSymbol({"::", {{"void", "hello-sourcetraildb", "()"}}});
    throw_on_error(writer, (id != 0));

    std::cout << "Closing writer" << std::endl;
    is_valid = writer.close();
    throw_on_error(writer, is_valid);

    return EXIT_SUCCESS;
}
