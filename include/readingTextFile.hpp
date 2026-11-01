#pragma once
#include <filesystem>
#include <string>

namespace minigit {

struct ReadResult {
    bool ok;
    std::string text;
    std::string error;
};

ReadResult readTextFile(const std::filesystem::path& filepath);

} // namespace minigit
