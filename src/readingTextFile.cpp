#include "readingTextFile.hpp"
#include <fstream>
#include <iterator>
#include <system_error>

namespace minigit {

ReadResult readTextFile(const std::filesystem::path& filepath) {
    std::error_code error;

    if (!std::filesystem::exists(filepath, error)) {
        return {false, "", "file does not exist: " + filepath.string()};
    }
    if (!std::filesystem::is_regular_file(filepath, error)) {
        return {false, "", "not a regular file: " + filepath.string()};
    }

    std::ifstream file(filepath);
    if (!file) {
        return {false, "", "cannot open file: " + filepath.string()};
    }

    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (file.bad()) {
        return {false, "", "error while reading file: " + filepath.string()};
    }

    return {true, text, ""};
}

} // namespace minigit
