#include "FileSystem.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <system_error>

namespace minigit {

// FS-1
std::optional<std::string> FileSystem::readBytes(const std::filesystem::path& path) const {
    // A missing file is not an error here: the caller gets "no value".
    // The error_code version of is_regular_file never throws.
    std::error_code error;
    if (!std::filesystem::is_regular_file(path, error)) {
        return std::nullopt;
    }

    
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        throw std::runtime_error("Cannot open file for reading: " + path.string());
    }

    
    std::ostringstream contents;
    contents << input.rdbuf();
    if (input.bad()) {
        throw std::runtime_error("Cannot read file: " + path.string());
    }
    return contents.str();
}

} 