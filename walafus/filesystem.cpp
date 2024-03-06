#include <iostream>
#include "filesystem.h"
#include "errors.h"

static bool default_error_handler(WalafusFilesystemErrorCode error_code, const char* error_description, void* error_object) {
    std::cout << error_description << std::endl;
    return true;
}

bool (*walafus_on_error)(WalafusFilesystemErrorCode error_code, const char* error_description, void* error_object)
    = default_error_handler;

Filesystem::Filesystem() {
    //
}

void Filesystem::add_source(BaseFilesystemSource* source) {
    sources.push_back(source);
}

std::unique_ptr<BaseFileStream> Filesystem::open_file_read(std::string_view filename) {
    for (auto i = sources.rbegin(); i != sources.rend(); ++i) {
        auto source = *i;
        auto file = source->open_file_read(filename);
        if (file)
            return file;
    }
    return nullptr;
}

std::vector<ubyte> Filesystem::read_file(std::string_view filename) {
    if (auto stream = open_file_read(filename); stream)
        return stream->read();
    return {};
}

ulong Filesystem::read_file(std::string_view filename, void* dst, ulong available_size) {
    if (auto stream = open_file_read(filename); stream)
        return stream->read_into(dst, available_size);
    return {};
}
