#include "SourcetrailDB/SourcetrailDBWriter.h"

#include <iostream>

void throw_on_error(const sourcetrail::SourcetrailDBWriter& writer,
                    bool is_valid) {
    if (!is_valid) {
        std::cout << "Error: " << writer.getLastError() << std::endl;
        throw std::runtime_error(writer.getLastError());
    }
}

int main() {
    sourcetrail::SourcetrailDBWriter writer;
    const std::string output_file = "hello_sourcetraildb.srctrldb";
    std::cout << "Opening writer " << output_file << std::endl;
    auto is_open = writer.open(output_file);
    throw_on_error(writer, is_open);

    std::cout << "Begin Transaction" << std::endl;
    auto is_begin_success = writer.beginTransaction();
    throw_on_error(writer, is_begin_success);

    std::cout << "Recording symbol" << std::endl;

    auto id =
        writer.recordSymbol({"::", {{"void", "hello_sourcetraildb", "()"}}});
    throw_on_error(writer, (id != 0));

    std::cout << "Commit Transaction" << std::endl;
    auto is_commited = writer.commitTransaction();
    throw_on_error(writer, is_commited);

    std::cout << "Closing writer" << std::endl;
    auto is_closed = writer.close();
    throw_on_error(writer, is_closed);

    return EXIT_SUCCESS;
}
